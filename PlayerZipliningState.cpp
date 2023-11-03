// [Credit] https://gameprogrammingpatterns.com/state.html
// Insiration for all state-based behavior here, in PlayerState, and in the various PlayerState-inheriting classes.

#include "precomp.h"
#include "PlayerZipliningState.h"

#include "Player.h"
#include "Hitbox.h"
#include "Shape.h"

#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerRecoilingState.h"

#include "LaunchCommand.h"

#include "KeyboardManager.h"
#include "Level.h"
#include "MapData.h"
#include "TilesetData.h"
#include "Vector2.h"
#include "TileCollision.h"

#include "PlayerStateRewindData.h"


PlayerZipliningState::PlayerZipliningState(Player& player, const MapTile& mapTile)
	: PlayerState{ player }
	, directionOfDescent_{ (mapTile.combinedCollisionLayer_ & TileCollisionID::ZiplineL)
		? Direction::Left
		: Direction::Right }
{	}


PlayerZipliningState::PlayerZipliningState(Player& player, PlayerStateRewindData* rewindData)
	: PlayerState{ player }
	, directionOfDescent_{ rewindData->vInt_ }
{	}


void PlayerZipliningState::Enter(const bool isRewindEntry)
{
	name_ = "Zipline";
	stateIndex_ = Player::StateName::Ziplining;
	player_.SetSprite(stateIndex_);

	player_.SetSpriteFacing(directionOfDescent_ == Direction::Left);
	
	// Player object interactions.
	player_.objectMask_ = ObjectId::Collectible | ObjectId::Enemy | ObjectId::EnemyAttack | ObjectId::Rope;

	// Player will follow zipline rail, can only collide with a wall (to stop).
	SetHitboxData();

	player_.GetAllTileMasks();

	player_.friction_ = player_.ziplineFriction_;

	// Y velocity acceleration is adjusted by angle of descent.
	player_.gravityModifier_ = sinf(30.0f * PI / 180);
	
	if (!isRewindEntry)
	{
		// Adjust player position to be just below zipline.
		SetPositionUnderZipline();
		
		// Halt vertical momentum once zipline is grabbed.
		player_.velocity_->y = 0.0f;
	}
}


void PlayerZipliningState::SetHitboxData()
{
	player_.hitboxes_[Player::HitboxRole::Head].Edit(5, 0, 11, 4,
		TileCollisionID::ZiplineL | TileCollisionID::ZiplineR,
		Hitbox::Mode::InteractPlus
	);

	player_.hitboxes_[Player::HitboxRole::Body].Edit(5, 0, 11, 16,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Core].Ignore();

	player_.hitboxes_[Player::HitboxRole::Tail].Edit(0, 9, 6, 7,
		TileCollisionID::Obstacle | TileCollisionID::Cliff | TileCollisionID::Ledge,
		Hitbox::Mode::Obstacle | Hitbox::Mode::Object
	);

	player_.hitboxes_[Player::HitboxRole::Feet].Ignore();
}


PlayerState* PlayerZipliningState::HandleInput(const KeyboardManager&, Command*&)
{
	// Player hit a wall while on zipline. Drop them.
	if (player_.isAgainstWall_)
	{
		player_.canGrabZipline_ = false;
		player_.ziplineDelay_ = { 0.8f };

		float2 contactNormal{ (directionOfDescent_ == Direction::Left ? -1.0f : 1.0f), 0.0f };
		return new PlayerRecoilingState{ player_, contactNormal };
	}


	// Can jump off zipline with jump. Maintains x velocity.

	// Can drop off zipline with down. Maintains x velocity.


	return nullptr;
}


void PlayerZipliningState::Update(float) 
{ 
	// No zipline command because it is so strongly tied to the state's variables.	
	player_.velocity_->x = 2.0f * player_.velocity_->y * (directionOfDescent_ == Direction::Left ? -1.0f : 1.0f);
}


// Restore movement settings.
void PlayerZipliningState::Exit() 
{
	player_.gravityModifier_ = 1.0f;
}


void PlayerZipliningState::StoreRewindData(PlayerStateRewindData* rewindData)
{
	rewindData->vInt_ = directionOfDescent_;
}


void PlayerZipliningState::SetPositionUnderZipline()
{
	int2 hitboxCenterBottom{ FindTileHitboxEnd() };
	
	if (hitboxCenterBottom.x != 0 || hitboxCenterBottom.y != 0)
	{
		player_.shape_->SetCenter(
			float2{ 
				1.0f * hitboxCenterBottom.x, 
				(1.0f * hitboxCenterBottom.y) + player_.hitboxes_[0].size_.y * 0.5f
			}
		);
	}
}


int2 PlayerZipliningState::FindTileHitboxEnd()
{
	// Check all contact mapTiles the head hitbox may have stored from collisions.
	for (int index{ 0 }; index < 4; ++index)
	{
		const MapTile* mapTile{ player_.hitboxes_[Player::HitboxRole::Head].overlappingTiles_[index] };

		if (mapTile == nullptr || mapTile->interactTile_ == nullptr)
		{
			return { 0, 0 };
		}

		// If the stored mapTile has a zipline interact tile, set self to be below the specific zipline hitbox touched.
		if (mapTile->interactTile_->hitboxes_[0]->tileLayer_ & (TileCollisionID::ZiplineL | TileCollisionID::ZiplineR))
		{
			int contactHitbox = player_.hitboxes_[Player::HitboxRole::Head].contactHitboxes_[index];

			return
			{
				mapTile->position.x + mapTile->interactTile_->hitboxes_[contactHitbox]->offset_.x + (mapTile->interactTile_->hitboxes_[contactHitbox]->size_.x >> 1),
				mapTile->position.y + mapTile->interactTile_->hitboxes_[contactHitbox]->offset_.y + mapTile->interactTile_->hitboxes_[contactHitbox]->size_.y
			};
		}
	}

	return { 0, 0 };
}