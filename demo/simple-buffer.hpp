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
        auto at(int x, int y) -> T &
        {
            return buffer[y * width + x];
        }
        auto at(int x, int y) const -> T const { return buffer[y * width + x]; }
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

    private:
        std::vector<T> buffer;
        int width, height;
    };
}