#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cc
{
    // Primitive format
    enum class PrimitiveFormat : GLenum
    {
        Byte = GL_BYTE,
        UBtyte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT,
        UShort = GL_UNSIGNED_SHORT,
        Int = GL_INT,
        UInt = GL_UNSIGNED_INT,
        Float = GL_FLOAT,
        Double = GL_DOUBLE
    };

    // Texture Format
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
    enum class TextureSourceFormat
    {
        R = GL_RED,
        Ri = GL_RED_INTEGER,
        RG = GL_RG,
        RGi = GL_RG_INTEGER,
        RGB = GL_RGB,
        RGBi = GL_RGB_INTEGER,
        RGBA = GL_RGBA,
        RGBAi = GL_RGBA_INTEGER,
        Depth = GL_DEPTH_COMPONENT,
        DepthStencil = GL_DEPTH_STENCIL
    };
} // namespace cc
