#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl-enum.hpp"
#include "state-object.hpp"
#include <resource/image.hpp>
namespace cc
{

    /**
     * @brief Texture basic interface. Describe the basic properties and the image source of texture.
     *
     */
    class Texture : public StateObject
    {
    public:
        Texture(TextureFormat format = TextureFormat::RGB, TextureType type = TextureType::Texture_2D);
        ~Texture();

        auto set_filter(TextureFilter filter) -> void;
        auto set_wrapping(TextureWrapping wrapping) -> void;

    protected:
        TextureFormat m_format;
        TextureType m_type;
        TextureFilter m_filter;
        TextureWrapping m_wrapping;
    };

    /**
     * @brief Texture2D meta information.
     *
     */
    class Texture2D : public Texture
    {
    public:
        /**
         * @brief Construct an empty Texture2D object
         *
         * @param width
         * @param height
         * @param format
         */
        Texture2D(int width, int height, TextureFormat format);
        Texture2D(ImagePtr image, TextureFormat format);
        Texture2D(std::string const &path, TextureFormat format);
        Texture2D(TextureFormat format, int width, int height,
                  TextureSourceFormat srcFormat, PrimitiveFormat srcType, const void *data);

        auto width() const -> int;
        auto height() const -> int;

    private:
        void allocate(TextureFormat format, int width, int height,
                      TextureSourceFormat srcFormat, PrimitiveFormat srcType, const void *data);
        std::vector<ImagePtr> images;
        int m_width, m_height;
    };
} // namespace cc
