#include "precomp.h"
#include "Shape.h"


// Position adjustments. Will update center after getting new value.
void Shape::SetPosition(const Vector2& position)
{
	start_ = position;

	UpdateCenter();
}


void Shape::MovePosition(const float2 deltaAmount)
{
	start_ += deltaAmount;

	UpdateCenter();
}


void Shape::UpdatePosition()
{
	start_.x = center_.x - (size_.x / 2.0f);
	start_.y = center_.y - (size_.y / 2.0f);

	UpdateEnd();
}


// Center adjustments. Will update position after getting new value.
void Shape::SetCenter(const Vector2& center)
{
	center_ = center;

	UpdatePosition();
}


void Shape::MoveCenter(const float2 deltaAmount)
{
	center_ += deltaAmount;

	UpdatePosition();
}


void Shape::UpdateCenter()
{
	center_.x = start_.x + (size_.x / 2.0f);
	center_.y = start_.y + (size_.y / 2.0f);

	UpdateEnd();
}


// Tail adjustments. Updated by position and center.
void Shape::UpdateEnd()
{
	end_.x = start_.x + size_.x - 1.0f;
	end_.y = start_.y + size_.y - 1.0f;
}


// Truncate all coordinates. Done after collision with obstacle.
void Shape::Truncate(float2 collisionNormals)
{
	if (collisionNormals.x != 0.0f)
	{
		start_.x = trunc(start_.x);
		end_.x = trunc(end_.x);
		center_.x = trunc(center_.x);
	}

	if (collisionNormals.y != 0.0f)
	{
		start_.y = trunc(start_.y) + 0.999f;
		end_.y = trunc(end_.y) + 0.999f;
		center_.y = trunc(center_.y) + 0.999f;
	}
}