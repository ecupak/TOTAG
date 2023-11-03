#pragma once


struct Vector2 // how to make template?
{
	// Constructors.
	Vector2() { }


	Vector2(float2 f)	
		: x{ f.x }
		, y{ f.y }
	{	}


	Vector2(float x, float y)		
		: x{ x }
		, y{ y }
	{	}


	Vector2 Absolute()
	{
		return { abs(x), abs(y) };
	}

	
	void Normalize()
	{
		float2 normalized{ normalize(float2{x, y}) };
		x = normalized.x;
		y = normalized.y;
	}


	// Return as float when accessed with no member method specified.
	// [Credit] https://stackoverflow.com/questions/49801418/default-return-of-class-instance-without-member-function
	explicit operator float2()
	{
		return float2{ x, y };
	}

	explicit operator float2() const
	{
		return float2{ x, y };
	}


	// Assignment
	Vector2 operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	Vector2 operator=(const float2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	// [Credit] https://www.learncpp.com/cpp-tutorial/overloading-operators-using-member-functions/
	// [Credit] https://www.learncpp.com/cpp-tutorial/overloading-operators-using-normal-functions/
		
	// ... (+) addition
	Vector2& operator+=(const Vector2& other)
	{	
		x += other.x;
		y += other.y;
		return *this;
	};

	Vector2& operator+=(const float2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& operator+=(const float& other)
	{
		x += other;
		y += other;
		return *this;
	}

	// ... (-) subtraction
	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	};

	Vector2& operator-=(const float2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& operator-=(const float& other)
	{
		x -= other;
		y -= other;
		return *this;
	}

	// ... (*) multiplication
	Vector2& operator*=(const Vector2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	};

	Vector2& operator*=(const float2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2& operator*=(const float& other)
	{
		x *= other;
		y *= other;
		return *this;
	}

	// ... (/) division
	Vector2& operator/=(const Vector2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	};

	Vector2& operator/=(const float2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2& operator/=(const float& other)
	{
		x /= other;
		y /= other;
		return *this;
	}

	// Components.
	float x{ 0.0f };
	float y{ 0.0f };
};



// Arithmetic and comparison operator overloads.
Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator+(const Vector2& real, const float2& other);
Vector2 operator+(const float2& other, const Vector2& real);
Vector2 operator+(const Vector2& real, const float& other);
Vector2 operator+(const float& other, const Vector2& real);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& real, const float2& other);
Vector2 operator-(const float2& other, const Vector2& real);
Vector2 operator-(const Vector2& real, const float& other);
Vector2 operator-(const float& other, const Vector2& real);
Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& real, const float2& other);
Vector2 operator*(const float2& other, const Vector2& real);
Vector2 operator*(const Vector2& real, const float& other);
Vector2 operator*(const float& other, const Vector2& real);
Vector2 operator/(const Vector2& lhs, const Vector2& rhs);
Vector2 operator/(const Vector2& real, const float2& other);
Vector2 operator/(const float2& other, const Vector2& real);
Vector2 operator/(const Vector2& real, const float& other);
Vector2 operator/(const float& other, const Vector2& real);
bool operator== (const Vector2& a, const Vector2& b);
bool operator==(const Vector2& v, const float& f);
bool operator==(const float& f, const Vector2& v);
bool operator!= (const Vector2& a, const Vector2& b);
bool operator!= (const Vector2& v, const float& f);
bool operator!= (const float& f, const Vector2& v);
bool operator>= (const Vector2& a, const Vector2& b);
bool operator>=(const Vector2& v, const float& f);
bool operator>=(const float& f, const Vector2& v);
bool operator<= (const Vector2& a, const Vector2& b);
bool operator<=(const Vector2& v, const float& f);
bool operator<=(const float& f, const Vector2& v);