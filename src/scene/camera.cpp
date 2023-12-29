#include "camera.hpp"

namespace nanami::util
{
	Camera::Camera(int w, int h, Vector3f const& pos, Vector3f const& u, Vector3f const& to, float n, float f, float fov)
		:m_width(w), m_height(h), m_position(pos), m_near(n), m_far(f), m_fov(fov), m_aspect(float(w) / h) {


	}

}