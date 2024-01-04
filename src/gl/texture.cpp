#include "texture.hpp"

namespace cc
{
    Texture::Texture(TextureFormat format, TextureType type) : m_format(format), m_type(type)
    {
    }
    Texture::~Texture()
    {
    }
    auto Texture::set_filter(TextureFilter filter) -> void
    {
        m_filter = filter;
    }
    auto Texture::set_wrapping(TextureWrapping wrapping) -> void
    {
        m_wrapping = wrapping;
    }

    // Texture2D
    void Texture2D::allocate(TextureFormat format, int width, int height,
                             TextureSourceFormat srcFormat, PrimitiveFormat srcType, const void *data)
    {
        }
    Texture2D::Texture2D(int width, int height, TextureFormat format)
    {
    }
    Texture2D::Texture2D(ImagePtr image, TextureFormat format)
    {
    }
    Texture2D::Texture2D(std::string const &path, TextureFormat format)
    {
    }
    Texture2D::Texture2D(TextureFormat format, int width, int height,
                         TextureSourceFormat srcFormat, PrimitiveFormat srcType, const void *data)
    {
    }

    auto Texture2D::width() const -> int
    {
        return m_width;
    }
    auto Texture2D::height() const -> int
    {
        return m_height;
    }
}