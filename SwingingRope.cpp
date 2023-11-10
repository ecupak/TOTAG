#include "precomp.h"
#include "SwingingRope.h"

#include "Vector2.h"
#include "Shape.h"
#include "Hitbox.h"

#include "Subject.h"
#include "Event.h"

#include "PlayerSwingingState.h"

#include "SwingingRopeRewindData.h"


// Shared resources for all rope objects.
float2 SwingingRope::sharedSurfaceSize_{ 5.4f * TILE_WIDTH, 3.0f * TILE_HEIGHT };


const float2& SwingingRope::GetSharedSurfaceSize()
{
	return sharedSurfaceSize_;
}


SwingingRope::SwingingRope()
	: GameObject{ ObjectClass::Rope, ObjectType::Prop, 0, 0, ObjectId::Rope, ObjectId::Player }
	, onPassengerChange_{ new Subject{} }
{
	shape_->size_ = sharedSurfaceSize_;

}


SwingingRope::~SwingingRope()
{
	delete onPassengerChange_;
}


void SwingingRope::Init(const float2 startTileToCenterOn, Sprite* sharedSprite)
{
	// Set base sprite to shared swinging rope sprite.
	// Will draw to this using PrepareSprite() before any pixel checks.
	sprite_ = sharedSprite;

	// Adjust start so the object's sprite is horizontally centered in the intended tile and its top is aligned with the top of the tile.
	float2 offset{
		(shape_->size_.x - TILE_WIDTH) * 0.5f,
		0
	};

	float2 anchorPoint{
		startTileToCenterOn.x - offset.x,
		startTileToCenterOn.y - offset.y
	};

	shape_->SetPosition(anchorPoint);

	// First node and first point are the anchor points.
	nodes_[0] = ec::float2{ sharedSurfaceSize_.x * 0.5f, 0.0f };
	points_[0] = ec::float2{ sharedSurfaceSize_.x * 0.5f, 0.0f };

	// Create hitboxes to go around each line segment.
	hitboxCount_ = pointCount_;
	hitboxes_ = new Hitbox[pointCount_];
	for (int index{ 0 }; index < pointCount_; ++index)
	{
		hitboxes_[index] = Hitbox{ 0, 0, 0, 0, 0, Hitbox::Mode::Object };
	}

	isActive_ = true;
}


void SwingingRope::Update(float deltaTime)
{
	// Do nothing if not onstage.
	if (!isActive_)
	{
		return;
	}

	// TODO: Put time into sin() to get -1 to 1 range. Use lerp easein/out 
	if (direction_ == 1)
	{
		time_ = min(timeUpper_, time_ + (hasPassenger_ ? 1.2f : 1.0f) * deltaTime);

		if (time_ >= timeUpper_)
		{
			direction_ *= -1;
		}
	}
	else
	{
		time_ = max(timeLower_, time_ - (hasPassenger_ ? 1.2f : 1.0f) * deltaTime);

		if (time_ <= timeLower_)
		{
			direction_ *= -1;
		}
	}

	UpdateNodes();
}


void SwingingRope::StoreRewindData(SwingingRopeRewindData& rewindData)
{
	// Skip storage step if not being updated.
	if (isActive_)
	{
		rewindData.direction_ = direction_;
		rewindData.time_ = time_;
		rewindData.hasPassenger_ = hasPassenger_;
	}

	GameObject::StoreRewindData(rewindData.gameObjectRewindData_);
}


void SwingingRope::Rewind(SwingingRopeRewindData& rewindData)
{
	direction_ = rewindData.direction_;
	time_ = rewindData.time_;
	hasPassenger_ = rewindData.hasPassenger_;

	GameObject::Rewind(rewindData.gameObjectRewindData_);

	// Must recalculate new positions.
	UpdateNodes();
}


void SwingingRope::Resume(SwingingRopeRewindData& rewindData)
{
	GameObject::Resume(rewindData.gameObjectRewindData_);
}


void SwingingRope::Draw(Surface* screen, int x, int y)
{
	// Line segments.
	for (int lineindex{ 0 }; lineindex < lineSegments_; ++lineindex)
	{		
		screen->Line(x + points_[lineindex].x, y + points_[lineindex].y, x + points_[lineindex + 1].x, y + points_[lineindex + 1].y, 0x4b692f);
	}
}


// Draw sprite to static Surface used by all swinging ropes to provide a pixel map to do a pixel check against.
void SwingingRope::PrepareSprite()
{
	Surface& sharedSurface{ *(sprite_->GetSurface()) };

	// Clear the surface.
	//may be faster to use memset(sharedSurface.pixels, 0x000000, sharedSurface.width * sharedSurface.height);
	sharedSurface.Clear(0x000000);

	for (int lineIndex{ 0 }; lineIndex < lineSegments_; ++lineIndex)
	{
		sharedSurface.Line(points_[lineIndex].x, points_[lineIndex].y, points_[lineIndex + 1].x, points_[lineIndex + 1].y, 0xFFFFFF);
	}
}


const int SwingingRope::GetAttachmentPoint(const float2& playerPos)
{
	// Get the position of the player relative to the position of the rope.
	float2 localPos{ playerPos.x - shape_->start_.x, playerPos.y - shape_->start_.y };

	// Find distance from each point to the local position of player and return the closest.
	int closestPoint{ -1 };
	float closestDistance{ 5'000.0f };

	// Don't allow attachment to anchor point.
	for (int pointIndex{ 1 }; pointIndex < pointCount_; ++pointIndex)
	{
		// Vector from point to player, and the (almost) magnitude of vector.
		float2 vectorFromPlayer{ points_[pointIndex].x - localPos.x, points_[pointIndex].y - localPos.y };
		float distanceFromPlayer{ static_cast<float>(pow(vectorFromPlayer.x, 2) + pow(vectorFromPlayer.y, 2)) };

		if (distanceFromPlayer < closestDistance)
		{
			closestDistance = distanceFromPlayer;
			closestPoint = pointIndex;
		}
	}

	return closestPoint;
}


const float2 SwingingRope::GetPositionOnRope(int& playerStartPoint, float& time)
{
	// If player is on the last point, there is nothing else to move to.
	if (playerStartPoint == pointCount_ - 1)
	{
		return { 
			shape_->start_.x + points_[playerStartPoint].x,
			shape_->start_.y + points_[playerStartPoint].y
		};
	}

	// Ensure time is within range and lerp to new position between points.
	time = clamp(time, 0.0f, 1.0f);
	ec::float2 position{};
	ec::Lerpf2(position, points_[playerStartPoint], points_[playerStartPoint + 1], time);
	
	// If reached end of line segment, set starting point to next point.
	if (time == 1.0f)
	{
		++playerStartPoint;
		time = 0.0f;
	}

	return { shape_->start_.x + position.x, shape_->start_.y + position.y };
}


void SwingingRope::SetHasPassenger(bool hasPassenger)
{
	hasPassenger_ = hasPassenger;

	onPassengerChange_->Notify(objectClass_, hasPassenger_ ? Event::OnTouch : Event::OnRelease);
}


// Private methods.

void SwingingRope::UpdateNodes()
{
	// Have the rope ease in and out of the ends
	float t{ (timeUpper_ + time_) / (timeUpper_ - timeLower_) };
	float smoothedTime{ ec::Lerpf(-1.0f, 1.0f, ec::EaseInOut(t)) };

	// The first node is attached to the ceiling and is not part of the update.
	ec::float2 previousNode{ nodes_[0] };

	for (int index{ 1 }; index < 4; ++index)
	{
		ec::float2& node{ nodes_[index] };

		float curveIntensity{ (3 + (2.0F * index)) * smoothedTime * PI / 20 };

		node.x = previousNode.x + TILE_WIDTH * sin(curveIntensity);

		node.y = previousNode.y + TILE_HEIGHT * sin((PI * 0.5f) + curveIntensity);

		previousNode = node;
	}

	// Update points along Bezier curve.
	for (int pointIndex{ 1 }; pointIndex < pointCount_; ++pointIndex)
	{
		ec::GetBezierPoint(points_[pointIndex], nodes_[0], nodes_[1], nodes_[2], nodes_[3], pointIndex / (pointCount_ - 1.0f));
	}

	// Update hitboxes to surround each line segment.
	int2 fullStart{ static_cast<int>(sharedSurfaceSize_.x), 0 };
	int2 fullEnd{ 0, 0 };
	for (int lineIndex{ 0 }; lineIndex < lineSegments_; ++lineIndex)
	{
		int2 start{
			static_cast<int>(round(min(points_[lineIndex].x, points_[lineIndex + 1].x))) - 1,
			static_cast<int>(round(min(points_[lineIndex].y, points_[lineIndex + 1].y))) - 1
		};

		int2 end{
			static_cast<int>(max(points_[lineIndex].x, points_[lineIndex + 1].x)) + 1,
			static_cast<int>(max(points_[lineIndex].y, points_[lineIndex + 1].y)) + 1
		};

		hitboxes_[1 + lineIndex].offset_ = start;
		hitboxes_[1 + lineIndex].size_ = { end.x - start.x, end.y - start.y };

		// Get the max and min start and end values so a better hitbox for the entire rope can be made.
		if (start.x < fullStart.x)
		{
			fullStart.x = start.x;
		}

		// Full start y will always be 0.

		if (end.x > fullEnd.x)
		{
			fullEnd.x = end.x;
		}

		if (end.y > fullEnd.y)
		{
			fullEnd.y = end.y;
		}
	}

	// First hitbox is always the full bounding box size.
	hitboxes_[0].offset_ = fullStart;
	hitboxes_[0].size_ = fullEnd - fullStart;
}