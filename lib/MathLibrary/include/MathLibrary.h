// https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170
#pragma once

#include <cmath> //  h: sqrtf(), floorf(), fabs(), fmodf(); cpp: isinf()


#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif


namespace ec
{		
	// See end of file for definition, overloads, and methods.
	struct __declspec(align(8)) float2;
	

	/// <summary>
	/// [Credit] Values from Wikipedia, cited from: Borland C/C++
	/// https://en.wikipedia.org/wiki/Linear_congruential_generator
	/// 
	/// Returns an int from [0 ... range]. 
	/// </summary>
	extern "C" MATHLIBRARY_API int GetRandom(const int range);


	/// <summary>
	/// Returns a float from [0 ... 1] using GetRandom(). 
	/// </summary>
	extern "C" MATHLIBRARY_API float GetRandomPercent();


	/// <summary>
	/// Returns a float2 with an x and y component equal to the vector's components divided by the vector's magnitude.
	/// </summary>
	extern "C" MATHLIBRARY_API void Normalize(float2& out, const float2 & vector);


	/// <summary>
	/// Returns a float of the dot product of vector A and vector B.
	/// </summary>
	extern "C" MATHLIBRARY_API float Dot(const float2 & vectorA, const float2 & vectorB);


	/// <summary>
	/// [Credit] http://www.sunshine2k.de/articles/coding/vectorreflection/vectorreflection.html
	/// 
	/// Returns a float2 of the reflected vector.
	/// </summary>
	extern "C" MATHLIBRARY_API void Reflect(float2 & out, const float2 & vector, float2 contactNormal);


	/// <summary>
	/// Returns a bool as true if the point lies within (but not on) the border of the AABB.
	/// </summary>
	extern "C" MATHLIBRARY_API bool AabbVsPoint(const float2 & aabbStart, const float2 & aabbSize, const float2 & point);


	/// <summary>
	/// [Credit] https://stackoverflow.com/questions/3982320/convert-integer-to-string-without-access-to-libraries
	/// 
	/// Fills the buffer with char's that represent the int value followed by a null character.
	/// (i.e., turns 564 into "564\0") 
	/// </summary>
	extern "C" MATHLIBRARY_API void GetCharFromInt(const int number, char buffer[]);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float that is the linearly interpolated value between startValue and endValue based on percentProgress.
	/// (e.g., Lerpf(0.0f, 10.0f, 0.2f) => 2.0f)
	/// 
	/// The parameter percentProgress should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float Lerpf(const float startValue, const float endValue, const float percentProgress);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Similar to Lerpf, but will linearly interpolate both the x and y components separately.
	/// (e.g, Lerpf2({0.0f, 10.0f}, {10.0f, 30.0f}, 0.5f) => {5.0f, 20.0f})
	/// </summary>
	extern "C" MATHLIBRARY_API void Lerpf2(float2 & out, const float2 startValue, const float2 endValue, const float percentProgress);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float that is the complement of the value.
	/// (e.g., Flip(0.6f) => 0.4f)
	/// 
	/// Intended use mainly with Lerp and the easing functions.
	/// The parameter value should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float Flip(const float value);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float from [0 ... 1] that is parameter time squared (exponential growth).
	/// This causes Lerp to increase exponentially at the end.
	/// 
	/// The parameter time should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float EaseIn(const float time);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float from [0 ... 1] that is the opposite of EaseIn().
	/// This causes Lerp to increase exponentially at the start.
	/// 
	/// The parameter time should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float EaseOut(const float time);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float from [0 ... 1] that is the combination of EaseIn() and EaseOut().
	/// This causes Lerp to experience the "slow growth" part of exponential growth
	/// at both the start and end, with almost linear movement between them.
	/// 
	/// The parameter time should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float EaseInOut(const float time);


	/// <summary>
	/// [Credit] https://www.febucci.com/2018/08/easing-functions/
	/// 
	/// Returns a float from [0 ... 1] that "peaks" at the midpoint (when parameter time is 0.5f).
	/// This causes Lerp to reach the end at the midpoint and return to the start at the end.
	/// 
	/// The parameter time should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API float Spike(const float time);


	/// <summary>
	/// [Credit] My friend Lynn, for helping explain the concept and reviewing algebra I had forgotten.
	/// 
	/// Returns a float2 that is the linearly interpolated point on a curve described by 
	/// parameter points P1 and P2 (the start and end, and influenced by parameter points P2 and P3, 
	/// and at a percent distance from the start given by parameter time.
	/// 
	/// The parameter time should be from [0 ... 1].
	/// </summary>
	extern "C" MATHLIBRARY_API void GetBezierPoint(float2 & out, const float2 & P1, const float2 & P2, const float2 & P3, const float2 & P4, const float time);


	// This float2 struct and overloads designed by Jacco Bikker, from his IGAD template: https://github.com/jbikker/tmpl8
	// Since this library was created for learning purposes, some supplied float2 functions have been omitted so they 
	// can be included in the DLL functions above. Some other functions are omitted as this is a primarily 2D math library.
	struct __declspec(align(8)) float2
	{
		float2() = default;
		float2(const float a, const float b) : x(a), y(b) {}
		float2(const float a) : x(a), y(a) {}
		
		float x{ 0.0f };
		float y{ 0.0f };
	};
	inline float2 make_float2(const float a, float b) { float2 f2{}; f2.x = a, f2.y = b; return f2; }

	inline float2 operator-(const float2& a) { return make_float2(-a.x, -a.y); }

	inline float2 operator+(const float2& a, const float2& b) { return make_float2(a.x + b.x, a.y + b.y); }
	inline float2 operator-(const float2& a, const float2& b) { return make_float2(a.x - b.x, a.y - b.y); }

	inline float2 operator*(const float2& a, const float2& b) { return make_float2(a.x * b.x, a.y * b.y); }
	inline void operator*=(float2& a, const float2& b) { a.x *= b.x; a.y *= b.y; }
	inline float2 operator*(const float2& a, float b) { return make_float2(a.x * b, a.y * b); }
	inline float2 operator*(float b, const float2& a) { return make_float2(b * a.x, b * a.y); }
	inline void operator*=(float2& a, float b) { a.x *= b; a.y *= b; }

	inline float2 operator/(const float2& a, const float2& b) { return make_float2(a.x / b.x, a.y / b.y); }
	inline void operator/=(float2& a, const float2& b) { a.x /= b.x;	a.y /= b.y; }
	inline float2 operator/(const float2& a, float b) { return make_float2(a.x / b, a.y / b); }
	inline void operator/=(float2& a, float b) { a.x /= b;	a.y /= b; }
	inline float2 operator/(float b, const float2& a) { return make_float2(b / a.x, b / a.y); }

	inline float sqrLength(const float2& v) { return Dot(v, v); }
	inline float length(const float2& v) { return std::sqrtf(Dot(v, v)); }
	inline float fracf(float v) { return v - std::floorf(v); }

	inline float2 fracf(const float2& v) { return make_float2(fracf(v.x), fracf(v.y)); }
	inline float2 floorf(const float2& v) { return make_float2(std::floorf(v.x), std::floorf(v.y)); }
	inline float2 fabs(const float2& v) { return make_float2(std::fabs(v.x), std::fabs(v.y)); }
	inline float2 fmodf(const float2& a, const float2& b) { return make_float2(std::fmodf(a.x, b.x), std::fmodf(a.y, b.y)); }
}