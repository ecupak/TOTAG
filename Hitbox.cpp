#include "precomp.h"
#include "Hitbox.h"

#include "MapData.h"


Hitbox::Hitbox() {}


Hitbox::Hitbox(const float shapeWidth)
	: parentWidth_{ static_cast<int>(shapeWidth) }
{	}


Hitbox::Hitbox(const float shapeWidth, const int xOffset, const int yOffset, 
	const int width, const int height, const int hitboxModeFlags)
	: size_{ width, height }
	, offset_{ xOffset, yOffset }
	, center_{ offset_.x + (size_.x * 0.5f), offset_.y + (size_.y * 0.5f) }
	, initialCenter_{ center_.x, center_.y }
	, parentWidth_{ static_cast<int>(shapeWidth) }
	, centerOffset_{ center_.x - (shapeWidth * 0.5f) }
	, mode_{ hitboxModeFlags }
{	}


void Hitbox::Edit(const int xOffset, const int yOffset, 
	const int width, const int height, const int tileMaskFlags, const int hitboxModeFlags)
{
	size_ = { width, height };
	offset_ = { xOffset, yOffset };

	center_ = { offset_.x + (size_.x * 0.5f), offset_.y + (size_.y * 0.5f) };
	initialCenter_ = { center_.x, center_.y };
	centerOffset_ = center_.x - (parentWidth_ * 0.5f);
	
	if (isFlipped_)
	{
		center_.x = center_.x - (2 * centerOffset_);
		offset_.x = static_cast<int>(center_.x - (size_.x * 0.5f));
	}

	// Assign bitflags unless -1, which means keep the same.
	if (tileMaskFlags >= 0)
	{
		tileMask_ = tileMaskFlags;
	}

	if (hitboxModeFlags >= 0)
	{
		mode_ = hitboxModeFlags;
	}
}


void Hitbox::Ignore()
{
	tileMask_ = 0;
	mode_ = 0;
}


void Hitbox::Flip(bool isFacingLeft)
{
	if (isFacingLeft && !isFlipped_)
	{
		center_.x = center_.x - (2 * centerOffset_);
		offset_.x = static_cast<int>(center_.x - (size_.x * 0.5f));
	}
	else if (!isFacingLeft && isFlipped_)
	{
		center_.x = initialCenter_.x;
		offset_.x = static_cast<int>(center_.x - (size_.x * 0.5f));
	}

	isFlipped_ = isFacingLeft;
}


void Hitbox::SetMask(int newCollisionMask)
{
	tileMask_ = newCollisionMask;
}


const float2 Hitbox::GetCenter() const
{
	return center_;
}