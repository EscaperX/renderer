#pragma once
#include <vector>
#include <iostream>
namespace cc
{
    template <typename T>
    class Buffer2D
    {
    public:
        Buffer2D() {}
        Buffer2D(int w, int h) : width(w), height(h)
        {
            buffer.resize(w * h);
        }
        Buffer2D(int w, int h, T const &val) : width(w), height(h), buffer(w * h, val)
        {
        }
        auto at(int w, int h) -> T &
        {
            // std::cout << w << " " << h << std::endl;
            return buffer[h * width + w];
        }
        auto at(int w, int h) const -> T const { return buffer[h * width + w]; }
        auto clear(T const &val = {}) -> void
        {
            for (auto &elem : buffer)
                elem = val;
        }
        auto resize(int w, int h) -> void
        {
            width = w;
            height = h;
            buffer.resize(w * h);
        }
        auto data() const -> const T * { return buffer.data(); }
        auto get_width() const -> int { return width; }
        auto get_height() const -> int { return height; }

    private:
        std::vector<T> buffer;
        int width, height;
    };
}