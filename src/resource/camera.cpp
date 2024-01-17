#include "camera.hpp"
#include "math/transform.hpp"

namespace cc
{
	Camera::Camera(int w, int h, Vector3f const &pos, Vector3f const &up, Vector3f const &to, float n, float f, float fov)
		: m_width(w), m_height(h), m_position(pos), m_near(n), m_far(f), m_fov(fov), m_aspect(float(w) / h)
	{
		m_to = glm::normalize(to);
		m_up = glm::normalize(up);
		m_right = glm::cross(m_to, m_up);

		m_project = persp_proj_matrix(m_fov, m_aspect, m_near, m_far);
		m_view = view_matrix(m_position, m_to, m_right, m_up);

		m_view_dirty = false;
	}

	auto Camera::view() -> Matrix4f
	{
		if (m_view_dirty)
		{
			m_view = view_matrix(m_position, m_to, m_right, m_up);
			m_view_dirty = false;
		}
		return m_view;
	}

	auto Camera::project() -> Matrix4f
	{
		if (m_proj_dirty)
		{
			m_proj_dirty = false;
			m_project = persp_proj_matrix(m_fov, m_aspect, m_near, m_far);
		}
		return m_project;
	}

	auto Camera::move_right(float dist) -> void
	{
		m_position += dist * m_right;
		m_view_dirty = true;
	}

	auto Camera::move_up(float dist) -> void
	{
		m_position += dist * m_up;
		m_view_dirty = true;
	}

	auto Camera::move_forward(float dist) -> void
	{
		m_position += dist * m_to;
		m_view_dirty = true;
	}
}
