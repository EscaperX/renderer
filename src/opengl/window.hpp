#pragma once

#include "application.hpp"
#include "resouce.hpp"
#include "helper_utils.hpp"

// #include "image.h"
#include <vector>

float data[1280 * 720 * 3] = {};
std::mutex data_mutex;
bool terminal_flag = false, updated = false;
std::condition_variable cv;

class Window : public Application
{
public:
    Window(int w = 1280, int h = 720) : Application(w, h)
    {
        // register vertices information for rectangle canvas
        _vao = opengl_wrapper::create_quad_vao();
        shader_program = opengl_wrapper::create_quad_program();
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
        glDeleteProgram(shader_program);
    }
    void renderFrame()
    {
        showFpsInWindowTitle();
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, _texture);

        std::unique_lock<std::mutex> lock(data_mutex);
        // for (int i = 0; i < _windowWidth * _windowHeight * 3; i += 3)
        // {
        //     data[i] = 1.0f;
        //     data[i + 1] = 0.0f;
        //     data[i + 2] = 0.0f;
        // }
        // std::this_thread::sleep_for(std::chrono::seconds(5));
        cv.wait(lock, []
                { return updated; });

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _windowWidth, _windowHeight,
                        GL_RGB, GL_FLOAT, (void *)(data));

        lock.unlock();
        updated = false;
        // data_mutex.unlock();

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
            terminal_flag = true;
            glfwSetWindowShouldClose(_window, true);
            return;
        }
    }

private:
    GLuint shader_program;
    GLuint _vao;
    GLuint _vbo;
    GLuint _texture;
    GLuint readFboId = 0;
};

class Window_Thread
{
public:
    Window_Thread(int w = 1280, int h = 720) : w(w), h(h)
    {
    }
    ~Window_Thread()
    {
        if (window)
            delete window;
    }
    void start()
    {
        thread = std::thread(&Window_Thread::run, this);
    }
    void run()
    {
        window = new Window(w, h);
        window->run();
    }
    void stop()
    {
        thread.join();
    }

private:
    int w, h;
    std::thread thread;
    Window *window = nullptr;
};
