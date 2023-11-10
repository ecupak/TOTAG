#pragma once

class Subject;

struct SwingingRopeRewindData;


class SwingingRope : public GameObject
{
public:
	SwingingRope();
	~SwingingRope();


	void Init(const float2 startTileToCenterOn, Sprite* sharedSprite);


	void Update(float deltaTime);


	// Manage rewind data.
	void StoreRewindData(SwingingRopeRewindData& rewindData);
	void Rewind(SwingingRopeRewindData& rewindData);
	void Resume(SwingingRopeRewindData& rewindData);


	void Draw(Surface* screen, int x, int y) override;


	void PrepareSprite() override;


	const int GetAttachmentPoint(const float2& startPoint);


	const float2 GetPositionOnRope(int& playerStartPoint, float& time);


	void SetHasPassenger(bool hasPassenger);


	Subject* onPassengerChange_{ nullptr };


	static const float2& GetSharedSurfaceSize();

private:
	void UpdateNodes();


	float timeLower_{ -1.0f };
	float timeUpper_{ 1.0f };
	float time_{ 0.0f };
	int direction_{ 1 };
	
	static constexpr int lineSegments_{ 7 };
	static constexpr int pointCount_{ lineSegments_ + 1 };

	ec::float2 nodes_[4]{ {0.0f, 0.0f} };

	// Use Bezier curve to generate points guided by the node path.
	ec::float2 points_[pointCount_]{ 0, 0 };

	// Full size the swinging rope can occupy. Used to created the shared surface the rope draws to during collision checks.
	static float2 sharedSurfaceSize_;

	bool hasPassenger_{ false };
};