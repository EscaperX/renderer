#pragma once

namespace cc
{
    template <typename T>
    class Buffer2D
    {
    public:
        Buffer2D(int w, int h, int c) : width(w), height(h)
        {
            buffer.resize(w * h * c);
        }
        auto at(int x, int y) -> T & { return buffer[y * width + x]; }
        auto at(int x, int y) const -> T const { return buffer[y * width + x]; }
        auto clear(T const &val) -> void
        {
            for (auto &elem : buffer)
                elem = val;
        }

    private:
        vector<T> buffer;
        int width, height;
    };
}