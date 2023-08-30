#pragma once
#include<vector>
#include "math-header.hpp"

namespace nanami
{
    namespace util
    {
        enum struct ImageLayout {

        };
        template<typename T>
        struct Image
        {
            Image() {}
            Image(int w, int h):width(w), height(h)
            {
                data.resize(w * h);
                memset(data.data(), 0, sizeof(T) * w *  h);
            }
            // Image(const Image& img) {
            //     width = img.width;
            //     height = img.height;
            //     data = img.data;
            // }

            int width, height;
            std::vector<T> data;
        };

        using Image3f = Image<math::Vector3f>;
        using Image1f = Image<float>;

    }
}
