#pragma once
class Utility
{
public:
	static int GetRandom2(const int range);

	static float GetRandomPercent2();

	static float2 GetReflectedVector2(const float2& velocity, float2 contactNormal);

	static void GetCharFromInt2(const int number, char buffer[]);

	static float Lerp2(const float startValue, const float endValue, const float percentProgress);

	static float2 Lerp2(const float2 startValue, const float2 endValue, const float percentProgress);

	static float Flip2(const float time);

	static float EaseIn2(const float time);
	
	static float EaseOut2(const float time);

	static float EaseInOut2(const float time);

	static float Spike2(const float t);

	static float2 GetBezierPoint2(const float2& P1, const float2& P2, const float2& P3, const float2& P4, const float time);
};