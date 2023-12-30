#include "transform.hpp"
#include "math-function.hpp"

namespace cc::math
{
	auto view_matrix(Vector3f const & pos, Vector3f const& to, Vector3f const & right, Vector3f const &up) -> Matrix4f
	{
		auto view = Matrix4f{
			right.x, up.x, -to.x, 0.f,
			right.y, up.y, -to.y, 0.f,
			right.z, up.z, -to.z, 0.f,
			-glm::dot(right, pos), -glm::dot(up, pos), -glm::dot(-to, pos), 0.0f
		};
		return view;
	}

	auto persp_proj_matrix(float fov, float aspect, float near, float far) -> Matrix4f
	{
		auto to_ortho = Matrix4f{1.0f};
		to_ortho[2][2] = (near + far) / near;
		to_ortho[3][2] = -far;
		to_ortho[2][3] = 1.0f / near;

		float height = tan(to_radian(fov) * 0.5) * near * 2;
		float width = height * aspect;
		auto proj = ortho_proj_matrix(width, height, near, far);
		return proj * to_ortho;
	}

	auto ortho_proj_matrix(int w, int h, float near, float far) -> Matrix4f
	{
		auto result = Matrix4f{1.0f};
		result[0][0] = 2.0f / w;
		result[1][1] = 2.0f / h;
		result[2][2] = 2.0f / (far - near);
		result[3][2] = -(far + near) / (far - near);
		return result;
	}
}