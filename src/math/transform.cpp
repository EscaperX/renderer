#include "transform.hpp"

namespace nanami::math
{
	auto view_matrix(Vector3f const & pos, Vector3f const& to, Vector3f const & right, Vector3f const &up) -> Matrix4f
	{
		auto view = Matrix4f{
			right.x, up.x, -to.x, 0.f,
			right.y, up.y, -to.y, 0.f,
			right.z, up.z, -to.z, 0.f,
			0.0f, 0.0f, 0.0f, 0.0f
		};
		return view;
	}

	auto persp_proj_matrix() -> Matrix4f
	{
		auto persp_proj = Matrix4f{1.0f};
		return persp_proj;
	}

	auto result_matrix(int w, int h, float near, float far) -> Matrix4f
	{
		auto result = Matrix4f{1.0f};
		result[0][0] = 2.0f / w;
		result[1][1] = 2.0f / h;
		result[2][2] = -2.0f / (far - near);

		return result;
	}
}