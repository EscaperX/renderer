#pragma once
#include <math/math-header.hpp>
#include <resource/model.hpp>

#include "vertex.hpp"
#include "fragment.hpp"
#include "simple-buffer.hpp"
#include "profile-stat.hpp"
#include "hiez.hpp"

#include <vector>

namespace cc
{

    class Raster
    {
    public:
        enum class RasterMode : int
        {
            Line,
            Triangle,
            None
        };
        enum class RasterMethod : int
        {
            Groundtruth,
            Scanline,
            AdvancedScanline
        };

        static auto instance() -> Raster &;
        auto initialize(math::Matrix4f const &model, math::Matrix4f const &view, math::Matrix4f const &project) -> void;
        auto make_drawcall(DrawCallBuffer const &buf) -> void;
        auto clear_color(math::Vector3u8i const &v = {0, 0, 0}) -> void;
        auto clear_depth() -> void;
        auto set_viewport(int w, int h) -> void;
        auto render_target(Buffer2D<math::Vector3u8i> *c, Buffer2D<double> *d) -> void;
        auto set_render_mode(RasterMode mode) -> void;
        auto set_render_method(RasterMethod method) -> void;
        // auto transform() -> void;
        // auto clip() -> void;
        auto rasterize() -> void;
        auto mutable_color() -> Buffer2D<math::Vector3u8i> &;
        auto depth_buffer() const -> Buffer2D<float> const *;
        auto get_color() const -> math::Vector3u8i const *;
        auto get_depth() const -> float const *;

        ProfileStat statistics;
        bool enable_zcull;

    private:
        Raster() = default;
        Raster(Raster const &) = delete;
        Raster &operator=(Raster const &) = delete;

        // settings
        RasterMode raster_mode;
        RasterMethod raster_method;

        // draw call
        std::vector<DrawCallBuffer> drawcalls;

        // camera
        math::Matrix4f model_mat;
        math::Matrix4f view_mat;
        math::Matrix4f project_mat;
        int viewport_height, viewport_width;

        // render target which can be replaced by framebuffer
        Buffer2D<math::Vector3u8i> color;
        Buffer2D<float> depth;
        Buffer2D<float> last_frame_depth;

        // internal buffer for vertex after vertex shader
        std::vector<Vertex> vertex_buffer;
        std::vector<Fragment> fragment_buffer;
        std::vector<uint32_t> index_buffer;

        // hiez
        std::unique_ptr<HieZbuffer> hiez;
    };
}