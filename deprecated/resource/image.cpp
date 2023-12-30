#include "image.hpp"

namespace cc::resource
{
    Channel::Channel(const std::string &name, int h, int w) : name(name), height(h), width(w)
    {
        data.reserve(h * w);
    }

    auto Channel::get_name() const -> const std::string &
    {
        return name;
    }

    auto Channel::get_data() const -> const std::vector<float> &
    {
        return data;
    }

    auto Channel::operator()(int x) const -> float
    {
        return data[x];
    }

    auto Channel::operator()(int x) -> float &
    {
        return data[x];
    }

    auto Channel::operator()(int x, int y) const -> float
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            exit(-1);
        }
        return data[y * width + x];
    }

    auto Channel::operator()(int x, int y) -> float &
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
        {
            exit(-1);
        }
        return data[y * width + x];
    }

    auto Channel::update_tile(int x, int y, int w, int h, const std::vector<float> &update_data) -> void
    {
        if (x < 0 || y < 0 || x + w >= width || y + h >= height)
        {
            return;
        }
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                (*this)(x + j, y + i) = update_data.at(i * w + j);
    }

}