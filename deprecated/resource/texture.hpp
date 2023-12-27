#pragma once

#include <optional>
#include "image.hpp"
#include <math/math-function.hpp>

namespace nanami::resource
{
    /**
     * \brief Mipmap generation
     * TODO: Complete the implementation of Mipmap
     */
    // template <typename T>
    // class Mipmap
    // {
    // public:
    //     Mipmap(const Image<T> &);
    //     auto create_mipmap(const Image<T> &) -> void;
    //     T lookup() const;
    // private:
    //     std::vector<Image<T>> images;
    //     /* data */
    // };

    /**
     * \brief
     * \tparam T Type of pixel. Supporting mainly Grey(float), RGB(vector3f), RGBA(vector4f)
     */
    template <typename T>
    class Texture
    {
    public:
        virtual ~Texture() = 0;
        virtual T sample(math::Vector2f uv) const = 0;
    };

    template <typename T>
    class Texture2D final : Texture<T>
    {
    public:
        Texture2D(int height = 1920, int width = 1080) : image(width, height) {}
        Texture2D(const Image<T> &img) : image(img)
        {
        }
        T sample(math::Vector2f uv) const
        {
            int x = int(uv.x * image.width - 0.5f) % image.width;
            int y = int(uv.y * image.height - 0.5f) % image.height;
            int id = y * image.width + x;
            return image.data.at(id);
        }

    private:
        Image<T> image;
        // std::optional<Mipmap<T>> mipmap;
    };

    template <typename T>
    class ConstantTexture final : Texture<T>
    {
        ConstantTexture(T c = T(1.0f)) : color((c)) {}
        ~ConstantTexture() {}
        T sample(math::Vector2f uv) const
        {
            return color;
        }

    private:
        T color;
    };

    template <typename T>
    class CheckerTexture final : Texture<T>
    {
    public:
        CheckerTexture(T c0 = T(0), T c1 = T(1)) : color0(c0), color1(c1) {}
        ~CheckerTexture() {}
        T sample(math::Vector2f uv) const
        {
            math::Vector2f local_uv{math::modulo(uv.x * uscale + uoffset, 1.0f), math::modulo(uv.y + vscale + voffset, 1.0f)};
            int x = 2 * math::modulo((int)(local_uv.x * 2), 2) - 1,
                y = 2 * math::modulo((int)(local_uv.y * 2), 2) - 1;

            if (x * y == 1)
            {
                return color0;
            }
            else
            {
                return color1;
            }
        }

    private:
        T color0, color1;
        float uscale, vscale;
        float uoffset, voffset;
    };
}