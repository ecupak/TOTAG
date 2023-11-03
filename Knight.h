#pragma once

struct KnightRewindData;


class Knight : public Enemy
{
public:
	Knight();


	void Init(const float2 startTileToCenterOn, Scorekeeper* scorekeeper, const Vector2* playerPosition);


	void Update(float deltaTime, float gravity);


	// Manage rewind data.
	void StoreRewindData(KnightRewindData& rewindData);
	void Rewind(KnightRewindData& rewindData);
	void Resume(KnightRewindData& rewindData);
	ActorState* GetResumedState(KnightRewindData& rewindData);


	// Collision management.
	void ResolveObstacleCollisions(const float2& contactNormals) override;
	void ResolveObjectCollisions() override;

	void Draw(Surface* screen, int x, int y) override;

	friend class KnightWalkingState;
	friend class KnightAttackingState;
	friend class KnightRecoilState;


private:
	enum SpriteSheet
	{
		Walking = 0,
		Attacking,
		Recoiling,
		SpriteSheetCount,
	};

	enum HitboxRole
	{
		Head = 0,
		Body,
		Arm,
		ObstacleDetector,
		GapDetector,
		PlayerDetector,
		HitboxRoleCount,
	};


	Action GetEnemyAction(float deltaTime) override;
	
	
	void ProcessTimers(float deltaTime) override;


	bool isSpriteBlinking_{ false };
	bool isInvulnerable_{ false };
	float maxInvulnerableTime_{ 0.8f };
	float elapsedInvulnerableTime_{ 0.0f };
};