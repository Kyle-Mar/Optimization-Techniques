#pragma once

namespace Utils{
	template<typename U>
	U lerp(const U& a, const U& b, float t) {
		return a + t * (b - a);
	}
}
