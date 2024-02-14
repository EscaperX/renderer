#include "hiez.hpp"
#include <algorithm>

namespace cc
{
    HieZbuffer::HieZbuffer(int width, int height)
    {
        int l = std::max(height, width);
        screen_box = AABB(0, height - 1, 0, width - 1, -100, 100);

        // align buffer size
        buffer_height = buffer_width = 1;
        while (buffer_height < height)
            buffer_height <<= 1;
        while (buffer_width < width)
            buffer_width <<= 1;

        initialize(nullptr);
    }
    auto HieZbuffer::initialize(Buffer2D<float> const *depth) -> void
    {
        zbuffers.clear();
        auto lowest_buffer = Buffer2D<float>(buffer_width, buffer_height, 0);
        if (depth == nullptr)
        {
            lowest_buffer.clear(FLT_MAX);
        }
        else
        {
            auto height = depth->get_height();
            auto width = depth->get_width();

            assert(buffer_height >= height && buffer_width >= width);
            // copy pre-frame depth
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                {
                    lowest_buffer.at(j, i) = depth->at(j, i);
                }
        }

        zbuffers.push_back(lowest_buffer);

        int id = 1, cur_height = buffer_height >> 1, cur_width = buffer_width >> 1;
        while (cur_height >= 1 && cur_width >= 1)
        {
            auto &last_buffer = zbuffers.back();
            auto cur_buffer = Buffer2D<float>(cur_width, cur_height);

            for (int i = 0; i < cur_height; i++)
                for (int j = 0; j < cur_width; j++)
                {
                    auto h = i << 1, w = j << 1;
                    auto z = std::max(std::max(last_buffer.at(w, h), last_buffer.at(w | 1, h)), std::max(last_buffer.at(w, h | 1), last_buffer.at(w | 1, h | 1)));
                    cur_buffer.at(j, i) = z;
                }

            zbuffers.push_back(std::move(cur_buffer));

            cur_height >>= 1, cur_width >>= 1;
        }
    }
    HieZbuffer::~HieZbuffer()
    {
    }

    // bool HieZbuffer::is_hidden(const Triangle &t)
    // {
    //     AABB box(t);
    //     return is_hidden(box);
    // }
    auto HieZbuffer::is_hidden(const AABB &box) const -> bool
    {
        if (!screen_box.intersect(box))
            return true;
        int b[2] = {std::min(screen_box.x[1], box.x[1]), std::min(box.y[1], screen_box.y[1])};
        int a[2] = {std::max(screen_box.x[0], box.x[0]), std::max(box.y[0], screen_box.y[0])};

        for (auto &buffer : zbuffers)
        {
            if (a[0] == b[0] && a[1] == b[1])
            {
                float z = buffer.at(a[0], a[1]);
                return z < box.z[0];
            }
            for (int i = 0; i < 2; i++)
                a[i] >>= 1, b[i] >>= 1;
        }
        return true;
    }
    auto HieZbuffer::get_z(int w, int h) const -> float
    {
        return zbuffers.front().at(w, h);
    }

    auto HieZbuffer::update(int w, int h, float z) -> void
    {
        zbuffers.front().at(w, h) = z;
        w >>= 1, h >>= 1;
        for (int id = 1; id < zbuffers.size(); id++)
        {
            int ww = w << 1, hh = h << 1;
            z = std::max(std::max(zbuffers[id].at(ww, hh), zbuffers[id].at(ww + 1, hh)), std::max(zbuffers[id].at(ww, hh + 1), zbuffers[id].at(ww + 1, hh + 1)));
            id--;

            if (zbuffers[id].at(w, h) > z)
                break;
            else
                zbuffers[id].at(w, h) = z;
            w >>= 1, h >>= 1;
        }
    }
    // void HieZbuffer::clear()
    // {
    //     for (auto &i : zbuffers)
    //     {
    //         i->clear();
    //     }
    // }
} // namespace cc
