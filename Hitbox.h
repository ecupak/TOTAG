#pragma once

// These are to be placed around objects to better approximate their shape.
// The /Shape/ structure represents the full object and its bounding box (not accurate).
// The /Hitbox/ structure should be constructed relative to the object that contains it. 
//  - Only use local coordinates. Do not manually update the start or center after creation.

struct MapTile;

class Hitbox
{
public:
	enum Mode
	{
		None			= 0b00'0000,
		Obstacle		= 0b00'0001,	// Use for collision with walls/ground.
		Map				= 0b00'0010,	// Receive reference of overlapped map tiles.
		Interact		= 0b00'0100,	// Receive reference of overlapped interact tiles (will only have the 1st hitbox if more than 1).
		InteractPlus	= 0b00'1000,	// Receive reference of overlapped interact tile and hitbox with pixel collision.
		Object			= 0b10'0000,	// Use for collision with other objects (requires sprite - will use pixel collision).
		Trigger			= 0b10'0000		// Receive reference of overlapped objects (does not notify other object of collision).
	};

	// Constructors.
	Hitbox();
	Hitbox(const float shapeWidth);
	Hitbox(const float shapeWidth, const int xOffset, const int yOffset, const int width, const int height, const int hitboxModeFlags);

	// Alter the position, size, and mode. Or Ignore by setting tilemask and mode to 0. Mainly for when states change.
	void Edit(const int xOffset, const int yOffset, const int width, const int height, const int tileMaskFlags, const int hitboxModeFlags);
	void Ignore();

	// Facing right is the assumed default.
	void Flip(bool isFacingLeft);

	// Can be changed by states.
	void SetMask(int newCollisionMask);

	// Return center. Mainly for raycasting against obstacles.
	const float2 GetCenter() const;

	// Position (local offset) and size.
	int2 offset_{ 0, 0 };
	int2 size_{ 0, 0 };

	// The mode(s) this hitbox operates in.
	int mode_{ 0b0 };		
	
	// The tile bitflags we are and care about.
	int tileMask_{ 0 };
	int tileLayer_{ 0 };

	// List of maptile that this hitbox overlaps.
	const MapTile* overlappingTiles_[4]{ nullptr, nullptr, nullptr, nullptr };	
	int contactHitboxes_[4]{ -1 };

private:
	float2 center_{ 0.0f, 0.0f };
	float2 initialCenter_{ 0.0f, 0.0f };
	int parentWidth_{ 0 };
	float centerOffset_{ 0 };	
	bool isFlipped_{ false };
};

