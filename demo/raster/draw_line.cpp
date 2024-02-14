#include "draw_line.hpp"

namespace cc
{
    void draw_line(math::Vector3i begin, math::Vector3i end, Buffer2D<math::Vector3u8i> &color)
    {
        auto x1 = begin.x;
        auto y1 = begin.y;
        auto x2 = end.x;
        auto y2 = end.y;

        if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
            return;
        if (x1 >= color.get_width() || x2 >= color.get_width() || y1 >= color.get_height() || y2 >= color.get_height())
            return;

        math::Vector3u8i line_color = {255, 255, 255};

        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

        dx = x2 - x1;
        dy = y2 - y1;
        dx1 = fabs(dx);
        dy1 = fabs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;

        if (dy1 <= dx1)
        {
            if (dx >= 0)
            {
                x = x1;
                y = y1;
                xe = x2;
            }
            else
            {
                x = x2;
                y = y2;
                xe = x1;
            }

            color.at(x, y) = line_color;

            for (i = 0; x < xe; i++)
            {
                x = x + 1;
                if (px < 0)
                    px = px + 2 * dy1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        y = y + 1;
                    else
                        y = y - 1;
                    px = px + 2 * (dy1 - dx1);
                }
                color.at(x, y) = line_color;
            }
        }
        else
        {
            if (dy >= 0)
            {
                x = x1;
                y = y1;
                ye = y2;
            }
            else
            {
                x = x2;
                y = y2;
                ye = y1;
            }
            color.at(x, y) = line_color;
            for (i = 0; y < ye; i++)
            {
                y = y + 1;
                if (py <= 0)
                    py = py + 2 * dx1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        x = x + 1;
                    else
                        x = x - 1;
                    py = py + 2 * (dx1 - dy1);
                }
                color.at(x, y) = line_color;
            }
        }
    }
}