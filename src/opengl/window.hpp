#pragma once

#include "application.hpp"
// #include "image.h"
#include <vector>

const char *vs_shader = "#version 330 core\n"
                        "layout (location = 0) in vec2 position;\n"
                        "layout (location = 1) in vec2 texCoords;\n"
                        "\n"
                        "out vec2 TexCoords;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
                        "    TexCoords = texCoords;\n"
                        "}\n";
const char *fs_shader = "#version 330 core\n"
                        "in vec2 TexCoords;\n"
                        "out vec4 color;\n"
                        "\n"
                        "uniform sampler2D screenTexture;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    color = texture(screenTexture, TexCoords);\n"
                        "}\n";

class Window : public Application
{
public:
    Window(int w = 1280, int h = 720) : Application(w, h)
    {
        data = new float[w * h * 3];
        // register vertices information for rectangle canvas
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(canvas_vertices), canvas_vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)(2 * sizeof(GLfloat)));
        glBindVertexArray(0);

        init_shader();
        glUseProgram(shader_program);
        glUniform1i(glGetUniformLocation(shader_program, "screenTexture"), 0);

        // register texture information
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);

        // Set texture interpolation methods for minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // // Set texture clamping method
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

        // Create the texture
        glTexImage2D(GL_TEXTURE_2D, // Type of texture
                     0,             // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,        // Internal colour format to convert to
                     w,             // Image width  i.e. 640 for Kinect in standard mode
                     h,             // Image height i.e. 480 for Kinect in standard mode
                     0,             // Border width in pixels (can either be 1 or 0)
                     GL_RGB,        // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_FLOAT,      // Image data type
                     NULL);         // The actual image data itself
        // register framebuffer
        glGenFramebuffers(1, &readFboId);
        glBindFramebuffer(GL_FRAMEBUFFER, readFboId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, _texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    ~Window()
    {
        delete[] data;
        glDeleteProgram(shader_program);
    }
    void init_shader()
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vs_shader, NULL);
        glCompileShader(vertexShader);
        // CheckShaderCompileSuccess(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fs_shader, NULL);
        glCompileShader(fragmentShader);
        // CheckShaderCompileSuccess(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // CheckShaderLinkSucess(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        shader_program = shaderProgram;
    }
    void renderFrame()
    {
        showFpsInWindowTitle();
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, _texture);

        for (int i = 0; i < _windowWidth * _windowHeight * 3; i += 3)
        {
            data[i] = 1.0f;
            data[i + 1] = 0.0f;
            data[i + 2] = 0.0f;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _windowWidth, _windowHeight,
                        GL_RGB, GL_FLOAT, (void *)(data));

        glUseProgram(shader_program);

        glBindVertexArray(_vao);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    void handleInput()
    {
        if (_keyboardInput.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE)
        {
            glfwSetWindowShouldClose(_window, true);
            return;
        }
    }

private:
    GLuint shader_program;

    const float canvas_vertices[24] = {
        -1.0f, 1.0f, 0.0f, 0.0f,  // 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, // 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f,  // 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, // 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f   // 1.0f, 1.0f
    };
    float *data;
    GLuint _vao;
    GLuint _vbo;
    GLuint _texture;
    GLuint readFboId = 0;
};
