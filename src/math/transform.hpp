#pragma once

#include "math-header.hpp"

namespace cc::math
{
	auto view_matrix(Vector3f const & pos, Vector3f const& to, Vector3f const & right, Vector3f const &up) -> Matrix4f;
	auto persp_proj_matrix(float fov, float aspect, float near, float far) -> Matrix4f;
	auto ortho_proj_matrix(int w, int h, float near, float far) -> Matrix4f;

}
