#pragma once
#include <cmath>

#ifndef 安全DELETE
	#define 安全DELETE(p) if(p){ delete (p);} (p) = nullptr;
	#define 安全DELETE_ARRAY(p) if(p){ delete[] (p);} (p) = nullptr;
#endif // !安全DELETE

namespace engine
{
	struct float2 {
		float x;
		float y;

		float2() { float2(0.0f, 0.0f); }
		float2(float x_, float y_) { x = x_; y = y_; }

		static float dot(float2 v0, float2 v1) { return v0.x * v1.x + v0.y * v1.y; }
		float length() { return std::sqrt(float2::dot(*this, *this)); }
		const float2 normalaize() { return *this /= length(); }

		const float2 operator + (const float2& v) const { return float2(this->x + v.x, this->y + v.y); }
		const float2 operator - (const float2& v) const { return float2(this->x - v.x, this->y - v.y); }
		const float2 operator * (float f) const { return float2(this->x * f, this->y * f); }
		float2& operator += (const float2& v) { this->x += v.x; this->y += v.y; return *this; }
		float2& operator /= (float f) { this->x /= f; this->y /= f; return *this; }

	};
}
;