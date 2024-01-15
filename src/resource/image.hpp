#pragma once

#include <cstdint>
#include <memory>
#include <filesystem>

namespace cc
{
    class Image
    {
    public:
        enum class ImageMetaType : uint32_t
        {
            int8,
            float32
        };
        Image(int w, int h, int c, ImageMetaType type);
        Image(std::filesystem::path const &path, ImageMetaType type, int c);
        ~Image();
        auto meta_type_size() const -> int;
        auto data() const -> uint8_t const *;
        auto data() -> uint8_t *;

    private:
        int m_width, m_height;
        int m_channels;
        ImageMetaType m_meta_type;
        uint8_t *m_data;
    };
    using ImagePtr = std::shared_ptr<Image>;
}