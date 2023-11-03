#pragma once


class Shape
{
public:
	// Position adjustments. Will update center after getting new value.
	void SetPosition(const Vector2& position);
	void MovePosition(const float2 deltaAmount);
	void UpdatePosition();

	// Center adjustments. Will update position after getting new value.
	void SetCenter(const Vector2& center);
	void MoveCenter(const float2 deltaAmount);
	void UpdateCenter();

	// Tail adjustments. Updated by position and center.
	void UpdateEnd();

	// Truncate all coordinates. Done after collision with obstacle.
	void Truncate(float2 collisionNormals);


	Vector2 start_{ 0.0f, 0.0f };
	Vector2 size_{ 0.0f, 0.0f };
	Vector2 center_{ 0.0f, 0.0f };
	Vector2 end_{ 0.0f, 0.0f };
};