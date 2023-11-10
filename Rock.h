#pragma once


struct RockRewindData;


class Rock : public GameObject
{
public:
	Rock();


	void SetSprite(Sprite* sprite);


	void Init(const float2 position, const float2 velocity);


	void Update(float deltaTime, float gravity);


	// Manage rewind data.
	void StoreRewindData(RockRewindData& rewindData);
	void Rewind(RockRewindData& rewindData);
	void Resume(RockRewindData& rewindData);


	void ResolveObstacleCollisions(const float2& contactNormals) override;


	void OnObjectCollision(GameObject* object) override;


private:
	void Bounce(const float2 contactNormals, const float elasticity);
	
	float lifetime_{ 0.0f };
};