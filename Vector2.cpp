#include "precomp.h"
#include "Vector2.h"


// ... (+) addition
Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret{ lhs };
	ret += rhs;
	return ret;
};


Vector2 operator+(const Vector2& real, const float2& other)
{
	Vector2 ret{ real };
	ret += other;
	return ret;
};


Vector2 operator+(const float2& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret += other;
	return ret;
};


Vector2 operator+(const Vector2& real, const float& other)
{
	Vector2 ret{ real };
	ret += other;
	return ret;
};


Vector2 operator+(const float& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret += other;
	return ret;
};


// ... (-) subtraction
Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret{ lhs };
	ret -= rhs;
	return ret;
};


Vector2 operator-(const Vector2& real, const float2& other)
{
	Vector2 ret{ real };
	ret -= other;
	return ret;
};


Vector2 operator-(const float2& other, const Vector2& real)
{
	Vector2 ret{ other };
	ret -= real;
	return ret;
};


Vector2 operator-(const Vector2& real, const float& other)
{
	Vector2 ret{ real };
	ret -= other;
	return ret;
};


Vector2 operator-(const float& other, const Vector2& real)
{
	Vector2 ret{ other };
	ret -= real;
	return ret;
};


// ... (*) multiplication
Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret{ lhs };
	ret *= rhs;
	return ret;
};


Vector2 operator*(const Vector2& real, const float2& other)
{
	Vector2 ret{ real };
	ret *= other;
	return ret;
};


Vector2 operator*(const float2& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret *= other;
	return ret;
};


Vector2 operator*(const Vector2& real, const float& other)
{
	Vector2 ret{ real };
	ret *= other;
	return ret;
};


Vector2 operator*(const float& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret *= other;
	return ret;
};

// ... (/) division
Vector2 operator/(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret{ lhs };
	ret /= rhs;
	return ret;
};


Vector2 operator/(const Vector2& real, const float2& other)
{
	Vector2 ret{ real };
	ret /= other;
	return ret;
};


Vector2 operator/(const float2& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret /= other;
	return ret;
};


Vector2 operator/(const Vector2& real, const float& other)
{
	Vector2 ret{ real };
	ret /= other;
	return ret;
};


Vector2 operator/(const float& other, const Vector2& real)
{
	Vector2 ret{ real };
	ret /= other;
	return ret;
};


// ... (==) equals
bool operator== (const Vector2& a, const Vector2& b)
{
	return (a.x == b.x && a.y == b.y);
}

bool operator==(const Vector2& v, const float& f)
{
	return (v.x == f && v.y == f);
}

bool operator==(const float& f, const Vector2& v)
{
	return (v.x == f && v.y == f);
}

// ... (!=) not equals
bool operator!= (const Vector2& a, const Vector2& b)
{
	return (a.x != b.x || a.y != b.y);
}

bool operator!= (const Vector2& v, const float& f)
{
	return (v.x != f || v.y != f);
}

bool operator!= (const float& f, const Vector2& v)
{
	return (v.x != f || v.y != f);
}

// ... (>=) greater than or equals
bool operator>= (const Vector2& a, const Vector2& b)
{
	return (a.x >= b.x || a.y >= b.y);
}


bool operator>=(const Vector2& v, const float& f)
{
	return (v.x >= f || v.y >= f);
}


bool operator>=(const float& f, const Vector2& v)
{
	return (f >= v.x || f >= v.y);
}

// ... (<=) less than or equals
bool operator<= (const Vector2& a, const Vector2& b)
{
	return (a.x <= b.x || a.y <= b.y);
}


bool operator<=(const Vector2& v, const float& f)
{
	return (v.x <= f || v.y <= f);
}


bool operator<=(const float& f, const Vector2& v)
{
	return (f <= v.x || f <= v.y);
}