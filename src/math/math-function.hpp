//
// Created by Stellaris on 11/19/2023.
//

#pragma once
#include "math-header.hpp"
#include <cmath>

namespace nanami::math {

	inline float modulo(float a, float b) {
		float r = ::fmodf(a, b);
		return (r < 0.0f) ? r+b : r;
	}
	inline int modulo(int a, int b) {
		auto r = a % b;
		return (r < 0) ? r+b : r;
	}
}