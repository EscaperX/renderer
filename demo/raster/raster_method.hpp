#pragma once
#include <math/math-header.hpp>
#include "vertex.hpp"
#include "fragment.hpp"

#include <vector>

namespace cc
{
    auto ground_truth_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer) -> std::vector<Fragment>;

    struct TriNode
    {
        float a, b, c, d;
        int dy;
        uint32_t id;
    };
    struct Edge
    {
        float x;
        float dx;
        int dy;
        uint32_t id;
    };
    struct ActiveEdgePair
    {
        // Coordinate x
        float xl;
        float zl;
        // dx stepping along y;
        float dxdyl;

        // Coordinate x
        float xr;
        float zr;
        // dx stepping along y;
        float dxdyr;

        // Shared attribute
        // start coordinate y
        int start_y;
        // Remaining step along y
        int dy_remain;
        // dz along x and along y
        float dzdx, dzdy;

        uint32_t tri_id;
        // uint32_t id, pair_id;
        void next_line()
        {
            xl += dxdyl;
            zl += dzdx * dxdyl + dzdy;

            xr += dxdyr;
            zr += dzdx * dxdyr + dzdy;

            dy_remain--;
        }
        bool terminate()
        {
            return dy_remain < 0;
        }
    };

    struct ActiveEdge
    {
        int xs, ys, xe, ye;
        float z;
        // dz along x and along y
        float dzdx, dzdy;
        int dx, dy, dx1, dy1, px, py;

        uint32_t tri_id, pair_id;

        ActiveEdge(int x, int y, float dzdx, float dzdy)
        {
            dx = xe - xs;
            dy = ye - ys;
            dx1 = fabs(dx);
            dy1 = fabs(dy);
            px = 2 * dy1 - dx1;
            py = 2 * dx1 - dy1;
        }
        void next_line()
        {
            // acute angle
            if (dy1 <= dx1)
            {

                bool y_change = false;
                while (!y_change)
                {
                    if (dx < 0)
                        xs--, z -= dzdx;
                    else
                        xs++, z += dzdx;
                    if (px < 0)
                        px = px + 2 * dy1;
                    else
                    {
                        ys++;
                        z += dzdy;
                        px = px + 2 * (dy1 - dx1);
                        y_change = true;
                    }
                }
            }
            else
            {
                ys++;
                z += dzdy;
                if (py <= 0)
                    py = py + 2 * dx1;
                else
                {
                    if (dx < 0)
                        xs--, z -= dzdx;
                    else
                        xs++, z += dzdx;
                    py = py + 2 * (dx1 - dy1);
                }
            }
        }
        bool terminate()
        {
            return ys > ye;
        }
    };

    auto scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>;

    auto simple_scanline_raster(std::vector<Vertex> const &vertex_buffer, std::vector<uint32_t> const &index_buffer, int height, int width) -> std::vector<Fragment>;

} // namespace cc
