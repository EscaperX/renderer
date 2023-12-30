//
// Created by Stellaris on 11/19/2023.
//

#pragma once
#include "math-header.hpp"
#include <cmath>

namespace cc::math {
	constexpr float MY_PI = 3.1415926;
	inline float modulo(float a, float b) {
		float r = ::fmodf(a, b);
		return (r < 0.0f) ? r+b : r;
	}
	inline int modulo(int a, int b) {
		auto r = a % b;
		return (r < 0) ? r+b : r;
	}
	inline float to_radian(float angle)
	{
		return angle / 180.0f * MY_PI;
	}
}