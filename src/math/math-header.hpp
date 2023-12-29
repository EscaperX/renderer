#pragma once
#include <cmath>
#if defined USE_EIGEN
#include <Eigen/Core>
#elif defined USE_GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif
namespace nanami
{
    namespace math
    {
#ifdef USE_EIGEN
        using Vector3f = Eigen::Vector3f;
        using Vector2f = Eigen::Vector2f;
        using Matrix3f = Eigen::Matrix3f;
        using Matrix4f = Eigen::Matrix4f;
#elif USE_GLM
    	using Vector3f = glm::vec3;
    	using Vector2f = glm::vec2;
    	using Matrix3f = glm::mat3;
    	using Matrix4f = glm::mat4;
#endif

    }
}
