#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cc
{
    enum class TextureType : GLenum
    {
        Texture_1D = GL_TEXTURE_1D,
        Texture_2D = GL_TEXTURE_2D,
        Texture_3D = GL_TEXTURE_3D,
        Texture_Buffer = GL_TEXTURE_BUFFER,
        Texture_2D_Array = GL_TEXTURE_2D_ARRAY,
        Texture_Cube = GL_TEXTURE_CUBE_MAP
    };
    enum class TextureFilter : GLint
    {
        Linear = GL_LINEAR,
        Nearest = GL_NEAREST
    };

    enum class TextureWrapping : GLint
    {
        ClampToEdge = GL_CLAMP_TO_EDGE,
        Repeat = GL_REPEAT
    };

    enum class TextureFormat : GLenum
    {
        R = GL_RED,
        R8 = GL_R8,
        R16f = GL_R16F,
        R32f = GL_R32F,

        RG = GL_RG,
        RG8 = GL_RG8,
        RG16f = GL_RG16F,
        RG32f = GL_RG32F,

        RGB = GL_RGB,
        RGB8 = GL_RGB8,

        RGBA8 = GL_RGBA8,
        RGBA16f = GL_RGBA16F,
        RGBA32f = GL_RGBA32F,

        Depth24 = GL_DEPTH_COMPONENT,
        Depth32f = GL_DEPTH_COMPONENT32F,

        Depth24_Stencil8 = GL_DEPTH24_STENCIL8,
        Depth32fSstencil8 = GL_DEPTH32F_STENCIL8,
    };
    /**
     * @brief Texture basic interface. Describe the basic properties and the image source of texture.
     *
     */
    class Texture
    {
    public:
        Texture();
        ~Texture();

    protected:
        TextureFormat m_texture_format;
        TextureType m_texture_type;
        TextureFilter m_texture_filter;
        TextureWrapping m_texture_wrapping;
    };

    class Texture2D : public Texture
    {
    public:
        };
} // namespace cc
