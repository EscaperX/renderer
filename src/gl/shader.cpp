#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace cc
{
    // GLSLShader::Source
    GLSLShader::Source &GLSLShader::Source::vertex(const std::string &shader_path)
    {
        m_vertex_shader = load_shader(shader_path);
        return *this;
    }

    GLSLShader::Source &GLSLShader::Source::geometry(const std::string &shader_path)
    {
        m_geometry_shader = load_shader(shader_path);
        return *this;
    }

    GLSLShader::Source &GLSLShader::Source::fragment(const std::string &shader_path)
    {
        m_fragment_shader = load_shader(shader_path);
        return *this;
    }

    GLSLShader::Source &GLSLShader::Source::compute(const std::string &shader_path)
    {
        m_compute_shader = load_shader(shader_path);
        return *this;
    }

    std::string GLSLShader::Source::load_shader(const std::string &shader_path)
    {
        std::ifstream shader_file;
        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Convert file contents into a std::string
            shader_file.open(shader_path);
            std::stringstream shader_string_stream;
            shader_string_stream << shader_file.rdbuf();
            shader_file.close();
            return shader_string_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ::PATH: " << shader_path << std::endl;
            throw;
        }
    }

    void GLSLShader::use() const
    {
        glUseProgram(m_handle);
    }

    void GLSLShader::introspect() const
    {
        const int max_name_length{64};
        const int num_parameters{2};
        GLint num_outputs{0};
        glGetProgramInterfaceiv(m_handle, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &num_outputs);

        static const GLenum properties[]{GL_TYPE, GL_LOCATION};
        GLint params[num_parameters];
        GLchar name[max_name_length];
        for (GLuint index{0}; index != num_outputs; ++index)
        {
            glGetProgramResourceName(m_handle, GL_PROGRAM_OUTPUT, index, sizeof(name), nullptr, name);
            glGetProgramResourceiv(m_handle, GL_PROGRAM_OUTPUT, index, num_parameters, properties, num_parameters, nullptr, params);
            std::cout << "Index: " << index << " is type: " << params[0] << " is named: " << name << " is at location: " << params[1] << std::endl;
        }
    }

    void GLSLShader::uniform(const std::string &name, GLboolean value) const
    {
        glUniform1i(glGetUniformLocation(m_handle, name.c_str()), value);
    }

    void GLSLShader::uniform(const std::string &name, GLuint value) const
    {
        glUniform1i(glGetUniformLocation(m_handle, name.c_str()), value);
    }

    void GLSLShader::uniform(const std::string &name, GLfloat value) const
    {
        glUniform1f(glGetUniformLocation(m_handle, name.c_str()), value);
    }

    void GLSLShader::uniform(const std::string &name, GLdouble value) const
    {
        glUniform1d(glGetUniformLocation(m_handle, name.c_str()), value);
    }

    void GLSLShader::uniform(const std::string &name, const glm::vec2 &vec2) const
    {
        glUniform2f(glGetUniformLocation(m_handle, name.c_str()), vec2.x, vec2.y);
    }

    void GLSLShader::uniform(const std::string &name, const glm::vec3 &vec3) const
    {
        glUniform3f(glGetUniformLocation(m_handle, name.c_str()), vec3.x, vec3.y, vec3.z);
    }

    void GLSLShader::uniform(const std::string &name, const glm::vec4 &vec4) const
    {
        glUniform4f(glGetUniformLocation(m_handle, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
    }

    void GLSLShader::uniform(const std::string &name, const glm::mat3 &value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLSLShader::uniform(const std::string &name, const glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    GLSLShader::GLSLShader(const Source &source)
    {
        m_handle = glCreateProgram();
        std::vector<GLuint> shader_handles;
        shader_handles.push_back(compile_shader(source.m_vertex_shader, GL_VERTEX_SHADER));
        shader_handles.push_back(compile_shader(source.m_geometry_shader, GL_GEOMETRY_SHADER));
        shader_handles.push_back(compile_shader(source.m_fragment_shader, GL_FRAGMENT_SHADER));
        shader_handles.push_back(compile_shader(source.m_compute_shader, GL_COMPUTE_SHADER));

        for (GLuint shader_handle : shader_handles)
        {
            if (shader_handle)
            {
                glAttachShader(m_handle, shader_handle);
            }
        }

        // if (separable)
        // {
        //     glProgramParameteri(m_handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
        // }

        glLinkProgram(m_handle);
        check_compile_errors(m_handle, GL_SHADER);

        for (GLuint shader_handle : shader_handles)
        {
            if (shader_handle)
            {
                glDetachShader(m_handle, shader_handle);
                glDeleteShader(shader_handle);
            }
        }
    }

    GLuint GLSLShader::compile_shader(const std::string &shader_string, GLenum shader_type) const
    {
        GLuint shader_handle{0};

        if (!shader_string.empty())
        {
            const char *shader_c_string{shader_string.c_str()};
            shader_handle = glCreateShader(shader_type);
            glShaderSource(shader_handle, 1, &shader_c_string, nullptr);
            glCompileShader(shader_handle);
            GLSLShader::check_compile_errors(shader_handle, shader_type);
        }

        return shader_handle;
    }

    void GLSLShader::check_compile_errors(const GLuint program_or_shader, const GLenum program_or_shader_type) const
    {
        GLint success;
        const GLuint log_length{1024};
        GLchar info_log[log_length];

        if (program_or_shader_type == GL_SHADER)
        {
            glGetProgramiv(program_or_shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(program_or_shader, log_length, nullptr, info_log);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << program_or_shader_type << "\n"
                          << info_log << std::endl;
            }
        }

        else
        {
            glGetShaderiv(program_or_shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(program_or_shader, log_length, nullptr, info_log);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << program_or_shader_type << "\n"
                          << info_log << std::endl;
            }
        }
    }
} // cc