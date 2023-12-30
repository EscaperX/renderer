#include "image.hpp"
#include <stb_image.h>
#include <string>

namespace cc
{
    auto Image::meta_type_size() const -> int
    {
        switch (m_meta_type)
        {
        case ImageMetaType::int8:
            return 1;
        case ImageMetaType::float32:
            return 4;
        default:
            std::runtime_error("Invalid meta type of image");
        }
    }
    Image::Image(int w, int h, int c, ImageMetaType type) : m_width(w), m_height(h), m_channels(c), m_meta_type(type)
    {
        m_data = new uint8_t[w * h * c * meta_type_size()];
    }
    Image::Image(std::filesystem::path const &path, ImageMetaType type, int c)
    {
        uint8_t *temp_data = reinterpret_cast<uint8_t *>(
            stbi_loadf(path.generic_string().c_str(), &m_width, &m_height, &m_channels, c));
        if (m_data == nullptr)
        {
            std::runtime_error(std::string("Failed to load image") + path.generic_string());
        }
        m_meta_type = type;
        size_t size = m_width * m_height * m_channels * meta_type_size();
        m_data = new uint8_t[size];
        memcpy(m_data, temp_data, size);
        stbi_image_free(temp_data);
    }
    Image::~Image()
    {
        if (m_data != nullptr)
            delete[] m_data;
    }
    auto Image::data() const -> uint8_t const *
    {
        return m_data;
    }
    auto Image::data() -> uint8_t *
    {
        return m_data;
    }

} // namespace cc
