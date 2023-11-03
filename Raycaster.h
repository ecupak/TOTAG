#pragma once

#include "Vector2.h"


class GameObject;
class Shape;
class Hitbox;


// 
// [Credit]
// https://www.youtube.com/watch?v=8JJ-4JgR7Dg
//
// Absolute godsend
//





class Raycaster
{
private:	// Ahead of public so struct can be easily used in methods.
	struct ExtendedHitbox
	{
		Vector2& offset_;
		Vector2& end_;
	};


public:
	// Returns true if a collision is found.
	// Parameters contactPoint, contactNormal, and timeToContact will be filled with collision data.
	bool RaycastToObject(const Vector2& origin, const Vector2& direction, const ExtendedHitbox& object, float& timeToContact, Vector2& contactNormal) const;

	bool FocusVsHitbox(const GameObject* const focus, const Hitbox& targetHitbox, const int2& targetPosition, float& timeToContact) const;
	 
	bool HitboxVsHitbox(const GameObject* const focusParent, Hitbox& focusHitbox, const Hitbox& targetHitbox, const int2& targetPosition,
		float& timeToContact, Vector2& contactNormal) const;
};