#pragma once

class KeyboardManager;
class Command;
class ActorState;
class RockPool;

struct PlayerRewindData;


class Player : public Actor
{
public:
	Player(RockPool* rockPool);
	~Player();


	void Place(const float2 startingPosition);


	void Reset();


	// Input and update delegate tasks to the current state class.
	void Update(float deltaTime, float gravity, KeyboardManager& km);


	// Manage rewind data.
	void StoreRewindData(PlayerRewindData* rewindData);
	void Rewind(PlayerRewindData* rewindData);
	void Resume(PlayerRewindData* rewindData);
	ActorState* GetResumedState(PlayerRewindData* rewindData);


	// Collision management.
	void ResolveObstacleCollisions(const float2& contactNormals) override;
	void ResolveObjectCollisions() override;
	

	// Debug - draws state name.
	void Draw(Surface* screen, int x, int y) override;


	const int GetCurrentHP() const;


	const int GetRockInventory() const;


	const bool IsAtLevelEnd() const;


	// TODO: MAKE STATES AND COMMANDS RECEIVE THE PARAMETERS THEY NEED INSTEAD OF HAVING FRIEND CLASSES.
	// State classes have special access to private methods.
	friend class PlayerBouncingState;
	friend class PlayerStandingState;
	friend class PlayerWalkingState;
	friend class PlayerJumpingState;
	friend class PlayerFallingState;
	friend class PlayerClimbingState;
	friend class PlayerZipliningState;
	friend class PlayerRecoilingState;
	friend class PlayerSwingingState;

	// Command classes have special access to private variables.
	friend class MoveCommand;
	friend class ClimbCommand;
	friend class JumpCommand;
	friend class ThrowRockCommand;

private:
	enum StateName
	{
		Standing = 0,
		Walking,
		Jumping,
		Falling,
		Climbing,
		Bouncing,
		Ziplining,
		Recoiling,
		Swinging,
		StateNameCount,
	};

	enum HitboxRole
	{
		Head = 0,
		Body,
		Core,
		Tail,
		Feet,
		HitboxRoleCount,
	};

	// Advance any active timers and resolve any that reach 0.
	void ProcessTimers(float deltaTime) override;


	// Checks if damage should be applied and returns if it was.
	const bool WasDamaged(const int damageAmount);


	// Add new tile flags to hitbox and actor tile masks while in same state.
	void AddTileMask(int hitboxFlags, const int tileFlags);


	// Tracks invulnerable status - invincible a short while after taking damage.
	bool isSpriteBlinking_{ false };
	bool isInvulnerable_{ false };
	float maxInvulnerableTime_{ 1.0f };
	float elapsedInvulnerableTime_{ 0.0f };


	// Variables affecting movement speed.	
	float ziplineFriction_{ 0.0f };
	float climbingSpeed_{ 2'000.0f };


	// Special substates.
	float ladderDelay_{ 0.0f };
	bool canGrabLadder_{ true };
	
	float ziplineDelay_{ 0.0f };
	bool canGrabZipline_{ true };

	float ropeDelay_{ 0.0f };
	bool canGrabRope_{ true };

	float throwDelay_{ 0.0f };
	bool canThrow_{ true };

	// Ammo ref.
	RockPool* rockPool_{ nullptr };
	int rockInventory_{ 3 };

	// Level end check.
	bool isAtLevelEnd_{ false };
};