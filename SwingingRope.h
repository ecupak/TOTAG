#pragma once

class Subject;

struct SwingingRopeRewindData;


class SwingingRope : public GameObject
{
public:
	SwingingRope();
	~SwingingRope();


	void Init(const float2 startTileToCenterOn);


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

	// Shared surface all swining ropes can draw to if they need to do a pixel check.
	static float2 size_;
	static Sprite* sharedSprite_;

	bool hasPassenger_{ false };
};