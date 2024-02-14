#include "camera.hpp"
#include "math/transform.hpp"
#include "math/math-function.hpp"
namespace cc
{
	auto Frustum::on_frustum(AABB const &bbx) const -> bool
	{
		auto on_plane = [&bbx](Plane const &plane) -> bool
		{
			auto extent = bbx.get_extent();
			auto center = bbx.get_center();
			auto abs_normal = glm::abs(plane.normal);
			auto r = glm::dot(extent, abs_normal);
			auto dist = plane.signed_distance(center);
			if (dist >= -r)
				return true;
			return false;
		};
		return on_plane(right) && on_plane(left) && on_plane(top) && on_plane(down) && on_plane(front) && on_plane(back);
	}

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
	auto Camera::frustum() const -> Frustum
	{
		Frustum frustum;

		auto half_height = m_far * tanf(glm::radians(m_fov) * .5f);
		auto half_width = half_height * m_aspect;
		auto to_far_plane = m_far * m_to;

		frustum.front = Plane{m_position + m_near * m_to, m_to};
		frustum.back = Plane{m_position + m_far * m_to, -m_to};
		frustum.top = Plane{
			m_position, glm::cross(m_right, to_far_plane - m_up * half_height)};
		frustum.down = Plane{
			m_position, glm::cross(to_far_plane + m_up * half_height, m_right)};
		frustum.left = Plane{
			m_position, glm::cross(m_up, to_far_plane + m_right * half_width)};
		frustum.right = Plane{m_position, glm::cross(to_far_plane - half_width * m_right, m_up)};
		return frustum;
	}
}
