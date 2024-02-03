#pragma once

#include "../math/math-header.hpp"

namespace cc
{
    using namespace math;

    class Camera
    {
    public:
        // struct Config
        // {
        //     int width;
        //     int height;
        //     Vector3f m_position;
        //     float near;
        //     float far;
        //     float fov;
        // };
        Camera(int w, int h, Vector3f const &pos = {0.0f, 0.0f, 2.0f},
               Vector3f const &u = {0.0f, 1.0f, 0.0f}, Vector3f const &to = {0.0f, 0.0f, -1.0f},
               float n = 0.5f, float f = 100.0f, float fov = 60.0f);
        auto view() -> Matrix4f;
        auto project() -> Matrix4f;

        auto move_right(float dist) -> void;
        auto move_up(float dist) -> void;
        auto move_forward(float dist) -> void;

        Vector3f m_position;
        Vector3f m_right;
        Vector3f m_up;
        Vector3f m_to;

        bool m_view_dirty;
        bool m_proj_dirty;

    private:
        float m_near, m_far, m_fov, m_aspect;
        int m_width, m_height;

        Matrix4f m_view;
        Matrix4f m_project;
    };

}