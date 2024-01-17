#pragma once

namespace cc
{
    class Application
    {
    public:
        Application(int r_w = 1024, int r_h = 1024) : m_resolution_height(r_h), m_resolution_width(r_w) {}
        virtual ~Application() {}
        virtual auto update() -> void {}
        virtual auto render() -> void {}
        virtual auto init() -> void {}
        auto viewport_width() const -> int { return m_viewport_width; }
        auto viewport_height() const -> int { return m_viewport_height; }
        auto set_viewport_width(int w) -> void { m_viewport_width = w; }
        auto set_viewport_height(int h) -> void { m_viewport_height = h; }

    protected:
        int m_viewport_width, m_viewport_height;
        int m_resolution_width, m_resolution_height;
        float m_last_timestep = 0;
    };
} // namespace cc
