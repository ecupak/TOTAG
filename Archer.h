#pragma once

class ArrowPool;

struct ArcherRewindData;


class Archer : public Enemy
{
public:
	Archer();

	void Init(const float2 startTileToCenterOn, Sprite* lockonSprite, Scorekeeper* scorekeeper, const Vector2* playerPosition, ArrowPool* arrowPool);

	
	void Update(float deltaTime, float gravity);


	// Manage rewind data.
	void StoreRewindData(ArcherRewindData& rewindData);
	void Rewind(ArcherRewindData& rewindData);
	void Resume(ArcherRewindData& rewindData);
	ActorState* GetResumedState(ArcherRewindData& rewindData);


	// Collision management.
	void ResolveObstacleCollisions(const float2& contactNormals) override;
	void ResolveObjectCollisions() override;

	void Draw(Surface* screen, int x, int y) override;

	friend class ArcherStandingState;
	friend class ArcherWalkingState;
	friend class ArcherAlertState;
	friend class ArcherShootingState;
	friend class ArcherRecoilingState;


private:
	enum SpriteSheet
	{
		Standing = 0,
		Walking,
		Alert,
		Shooting,
		Recoiling,
		SpriteSheetCount,
	};

	enum HitboxRole
	{
		Head = 0,
		Body,
		PersonalSpace,
		ObstacleDetector,
		GapDetector,
		PlayerDetector,
		HitboxRoleCount,
	};

	
	void ProcessTimers(float deltaTime) override;


	Action GetEnemyAction(float deltaTime) override;

	ArrowPool* arrowPool_{ nullptr };
	float attackDelay_{ 0.0f };

	Sprite* lockonSprite_{ nullptr };	// does not own.
	int lockonSpriteFrame_{ -1 };
};