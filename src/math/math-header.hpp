#pragma once
#define USE_EIGEN
#include <cmath>
#if defined USE_EIGEN
#include <Eigen/Core>
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
#endif
    }
}
