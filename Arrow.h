#pragma once

struct ArrowRewindData;


class Arrow : public GameObject
{
public:
	Arrow();
	~Arrow();

	void Init(const float2& position, const int archerWidth, const float2& direction, const float speed);


	void Update(float deltaTime, float gravity);


	// Manage rewind data.
	void StoreRewindData(ArrowRewindData& rewindData);
	void Rewind(ArrowRewindData& rewindData);
	void Resume(ArrowRewindData& rewindData);


	void ResolveObstacleCollisions(const float2& contactNormals) override;


	void OnObjectCollision(GameObject* object) override;


	void Draw(Surface* screen, int x, int y) override;


	bool isMarkedForDeactivation_{ false };


private:
	void SetArrowStartingPosition(const float2& position, const int archerWidth, const float2& direction);


	void CalculateArrowEnd();


	float lifetime_{ 0.0f };

	static Sprite* arrowSprite_;

	bool isStuck_{ false };

	Vector2* prevPosition_{ nullptr };

	uint tipLength_{ 3 };
	uint shaftLength_{ 20 };

	float2 tipEnd_{ 0.0f,  0.0f };
	float2 shaftEnd_{ 0.0f, 0.0f };

	uint tipColor{ 0xF2F7F6 };
	uint shaftColor{ 0x926155 };

};