#include "precomp.h"
#include "Raycaster.h"

#include "GameObject.h"
#include "Shape.h"
#include "Hitbox.h"

#include "Vector2.h"
#include "TilesetData.h"


// Returns true if a collision is found.
// Parameters contactPoint, contactNormal, and timeToContact will be filled with collision data.
bool Raycaster::RaycastToObject(const Vector2& origin, const Vector2& direction, const ExtendedHitbox& object, float& timeToContact, Vector2& contactNormal) const
{
	// Find distance from obstacle corners to start of ray. Divide by ray length to get % distance along ray.
	Vector2 tNear{ (object.offset_ - origin) / direction };
	Vector2 tFar{ (object.end_ - origin) / direction };


	// Catch divide-by-zero errors here (only happens when numerator is also 0, otherwise we get (+/- inf). Return false.
	if (isnan(tNear.x) || isnan(tNear.y) || isnan(tFar.x) || isnan(tFar.y))
	{
		return false;
	}


	// If the internal values are in the wrong order (x > y), swap them.
	// tNear should always be, well, nearer to the origin.
	if (tNear.x > tFar.x)
	{
		tNear.x += tFar.x;
		tFar.x = tNear.x - tFar.x;
		tNear.x -= tFar.x;
	}

	if (tNear.y > tFar.y)
	{
		tNear.y += tFar.y;
		tFar.y = tNear.y - tFar.y;
		tNear.y -= tFar.y;
	}


	// If either is true, then we did not intersect the object.
	if (tNear.x > tFar.y || tNear.y > tFar.x)
	{
		return false;
	}


	// Find the "time" to enter and exit the object.
	timeToContact = max(tNear.x, tNear.y);
	float tHitFar{ min(tFar.x, tFar.y) };


	// Ignore if entire collision happens behind origin.
	if (tHitFar <= 0.0f)
	{
		return false;
	}


	// Find entry and exit points from (0, 0). Add the untruncated origin of the focus to them after returning to the collision manager.
	//entryPoint = (timeToContact * direction);
	//endPoint = (min(tHitFar, 1.0f) * direction); // If direction was not long enough to reach end, find point it actually stopped at inside object.


	// Get normal of the side we made contact with. This will roughly face towards our origin.
	// If we hit a vertical side last (it has a greater t-value) then it is the contact point.
	if (tNear.x > tNear.y)
	{
		contactNormal = Vector2{ (direction.x > 0 ? -1.0f : 1.0f), 0.0f };
	}
	else
	{
		contactNormal = Vector2{ 0.0f, (direction.y > 0 ? -1.0f : 1.0f) };
	}

	return true;
}

/*

// Extract the raycast origin and direction from the focus' position and velocity and then cast against the object.
bool Raycaster::DynamicRaycastToObject(GameObject* focus, const GameObject& object, 
	Vector2& entryPoint, Vector2& endPoint, Vector2& contactNormal, float& timeToContact)
{
	// If the focus isn't moving, it won't have any length to its ray direction. If it's not moving, it can't collide.
	// If it's somehow in an object and not moving, that's a different issue.
	// Testing to only process if the movement would take us to a new pixel.
	if (focus->velocity_ == 0.0f)
	{
		return false;
	}

	// We must expand the border of the object. Our raycast is a single line that doesn't have width.
	// It only tracks where the center of the focus can go. Any collision will set the center to that point.
	// By expanding the object border, we will correctly collide when we get within distance = focus.width/2.
	Shape extendedShape;

	extendedShape.size_ = float2{
		object.shape_.size_.x + focus->shape_.size_.x,
		object.shape_.size_.y + focus->shape_.size_.y
	};

	extendedShape.SetPosition(float2{
		object.shape_.start_.x - (focus->shape_.size_.x / 2.0f),
		object.shape_.start_.y - (focus->shape_.size_.y / 2.0f)
	});

	extendedShape.end_ += 1.0f;

	// Must use truncated values or everything goes pear-shaped if using decimal - it just doesn't calculate correctly.
	Vector2 truncatedOrigin{ trunc(focus->shape_.center_.x), trunc(focus->shape_.center_.y) };

	// Check against object.
	return RaycastToObject(truncatedOrigin, focus->velocity_, extendedShape, entryPoint, endPoint, contactNormal, timeToContact);
}


// See above for execution details. In this overload, a single hitbox of the focus is checked for collision.
bool Raycaster::DynamicRaycastToObject(const Hitbox& hitbox, GameObject* focus, const GameObject& object,
	Vector2& entryPoint, Vector2& endPoint, Vector2& contactNormal, float& timeToContact)
{
	if (focus->velocity_ == 0.0f)
	{
		return false;
	}

	Shape extendedShape;

	extendedShape.size_ = float2{
		object.shape_.size_.x + hitbox.size.x,
		object.shape_.size_.y + hitbox.size.y
	};

	extendedShape.SetPosition(float2{
		object.shape_.start_.x - (hitbox.size.x / 2.0f),
		object.shape_.start_.y - (hitbox.size.y / 2.0f)
		});

	extendedShape.end_ += 1.0f;

	Vector2 truncatedOrigin{
		trunc(focus->shape_.start_.x) + hitbox.GetCenter().x,
		trunc(focus->shape_.start_.y) + hitbox.GetCenter().y
	};

	// Check against object.
	return RaycastToObject(truncatedOrigin, focus->velocity_, extendedTargetHitbox, entryPoint, endPoint, contactNormal, timeToContact);
}

*/


// Player vs a tile hitbox.
bool Raycaster::FocusVsHitbox(const GameObject* const focus, const Hitbox& targetHitbox, const int2& targetPosition, float& timeToContact) const
{
	if (*focus->velocity_ == 0.0f)
	{
		return false;
	}
	
	// Extend target hitbox by half of focus on all sides. New size is original hitbox + focus.
	Vector2 targetStart{
		targetPosition.x - (focus->shape_->size_.x * 0.5f),
		targetPosition.y - (focus->shape_->size_.y * 0.5f)
	};

	Vector2 targetEnd{
		targetStart.x + focus->shape_->size_.x + targetHitbox.size_.x,
		targetStart.y + focus->shape_->size_.y + targetHitbox.size_.y
	};

	ExtendedHitbox extendedTargetHitbox{ targetStart, targetEnd };

	// Truncate origin of ray because the math does not jive with an origin with decimal.
	Vector2 truncatedOrigin{ trunc(focus->shape_->center_.x), trunc(focus->shape_->center_.y) };

	// Check against object. Supply dummy out-value that is not used by caller.
	Vector2 dummyContactNormal{ 0.0f, 0.0f };

	return RaycastToObject(truncatedOrigin, *focus->frameVelocity_, extendedTargetHitbox, timeToContact, dummyContactNormal);
}


// A player hitbox vs a tile hitbox.
bool Raycaster::HitboxVsHitbox(const GameObject* const focusParent, Hitbox& focusHitbox, const Hitbox& targetHitbox, const int2& targetPosition,
	float& timeToContact, Vector2& contactNormal) const
{
	if (*focusParent->velocity_ == 0.0f)
	{
		return false;
	}

	// Extend target hitbox by half of focus on all sides. New size is original hitbox + focus.
	Vector2 targetStart{
		targetPosition.x - (focusHitbox.size_.x * 0.5f),
		targetPosition.y - (focusHitbox.size_.y * 0.5f)
	};

	Vector2 targetEnd{
		targetStart.x + focusHitbox.size_.x + targetHitbox.size_.x,
		targetStart.y + focusHitbox.size_.y + targetHitbox.size_.y
	};

	ExtendedHitbox extendedTargetHitbox{ targetStart, targetEnd };

	// Truncate origin of ray because the math does not jive with an origin with decimal.
	
	Vector2 truncatedOrigin{
		trunc(focusParent->shape_->start_.x) + focusHitbox.GetCenter().x,
		trunc(focusParent->shape_->start_.y) + focusHitbox.GetCenter().y
	};
	
/*
	Vector2 origin{
		focusParent->shape_->start_.x + focusHitbox.GetCenter().x,
		focusParent->shape_->start_.y + focusHitbox.GetCenter().y
	};
*/
	// Check against object.
	return RaycastToObject(truncatedOrigin, *focusParent->frameVelocity_, extendedTargetHitbox, timeToContact, contactNormal);
}