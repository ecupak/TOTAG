#include "precomp.h"
#include "Utility.h"


#include "Vector2.h"


// [Credit] Values from Wikipedia, cited from: Borland C/C++
// https://en.wikipedia.org/wiki/Linear_congruential_generator
int Utility::GetRandom2(const int range)
{
	static int seed{ 9 };
	static int increment{ 1 };
	static const int multiplier{ 22'695'477 };
	static const size_t modulus{ 4'000'000'001 };
	
	seed = ((seed * multiplier) + increment) % modulus;

	return seed % range;
}


float Utility::GetRandomPercent2()
{
	return GetRandom2(100) / 100.0f;
}


// [Credit] http://www.sunshine2k.de/articles/coding/vectorreflection/vectorreflection.html
float2 Utility::GetReflectedVector2(const float2& velocity, float2 contactNormal)
{
	// If |contactNormal| = 0 or there were no obstacle collisions, assume normal is the opposite of y-velocity.
	if ((contactNormal.x == 0.0f && contactNormal.y == 0.0f) || isinf(contactNormal.x) || isinf(contactNormal.y))
	{
		contactNormal.y = (velocity.y < 0.0f ? 1.0f : -1.0f);
	}
	else
	{
		contactNormal = normalize(contactNormal);
	}

	// Return the reflected vector.	
	float doubleDot{ 2.0f * dot(velocity, contactNormal) };
	return {
		velocity.x - (doubleDot * contactNormal.x),
		velocity.y - (doubleDot * contactNormal.y)
	};	
}


// [Credit] https://stackoverflow.com/questions/3982320/convert-integer-to-string-without-access-to-libraries
void Utility::GetCharFromInt2(const int number, char buffer[])
{
	int index{ 0 };
	int num{ number };

	while (num != 0)
	{
		// Convert last digit to value and add to char of '0' as starting offset for numerals as char.
		buffer[index] = num % 10 + '0';

		// Integer division. Drop last digit.
		num = num / 10;
		
		++index;
	}

	// Add terminator to end of char array.
	buffer[index] = '\0';

	// Swap without temp. Reverse the order of the buffer since it was loaded with last digit first.
	for (int t = 0; t < index / 2; t++)
	{
		buffer[t] ^= buffer[index - t - 1];
		buffer[index - t - 1] ^= buffer[t];
		buffer[t] ^= buffer[index - t - 1];
	}

	// Handle special case of 0.
	if (number == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
	}
}


// [Credit] https://www.febucci.com/2018/08/easing-functions/
// For lerp, easings, and flip
float Utility::Lerp2(const float startValue, const float endValue, const float percentProgress)
{
	return (startValue + (endValue - startValue) * percentProgress);
}


float2 Utility::Lerp2(const float2 startValue, const float2 endValue, const float percentProgress)
{
	return {
		startValue.x + (endValue.x - startValue.x) * percentProgress,
		startValue.y + (endValue.y - startValue.y) * percentProgress
	};
}


// Useful for percentages. Gives the complement of the value.
float Utility::Flip2(const float t)
{
	return 1.0f - t;
}


// Returns a value that increases exponentially.
float Utility::EaseIn2(const float t)
{
	return t * t;
}


// Returns a value that decreases exponentially.
float Utility::EaseOut2(const float t)
{
	// Get the 'EaseIn' of the Flip() of t, and then Flip() that result.
	return Flip2(sqrf(Flip2(t)));
}


// Returns a value that both increases and decreases exponentially.
float Utility::EaseInOut2(const float t)
{
	return Lerp2(EaseIn2(t), EaseOut2(t), t);
}


// Returns a value that reaches the destination at t = 50% and then reverses to the start.
float Utility::Spike2(const float t)
{
	if (t <= 0.5f)
	{
		return EaseIn2(t);
	}
	else
	{
		return EaseIn2(Flip2(t) / 0.5f);
	}
}


// [Credit] Lynn for helping explain the concept and the math I had forgotten.
float2 Utility::GetBezierPoint2(const float2& P1, const float2& P2, const float2& P3, const float2& P4, const float t)
{
	return 
		P1 * ((-1 * t * t * t) + ( 3 * t * t) + (-3 * t) + 1) +
		P2 * (( 3 * t * t * t) + (-6 * t * t) + ( 3 * t))+
		P3 * ((-3 * t * t * t) + ( 3 * t * t))+
		P4 *  (     t * t * t);
}