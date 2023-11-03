#pragma once

class Shape;
class Observer;

struct CameraRewindData;

enum class ObjectClass;
enum class Event;


class Camera : public Observer
{
public:
	enum class Mode
	{
		Normal,
		Parallax,
	};


	Camera();
	~Camera();

	// Get reference to the level and calculate edges.
	void Initialize(Surface* stagingScreen);

	void Update(const float deltaTime);

	void SetFocus(GameObject* focus);

	void SetLevelBounds(const int2& mapTileSize);

	int2 margin_{ 5 * TILE_WIDTH, 5 * TILE_HEIGHT };

	void ChangeZoomLevel(const float deltaZoomLevel);
	void SetZoomLevel(const float newZoomLevel);
	const float GetZoomLevel() const;

	const int2 GetCameraStart(const Mode cameraMode) const;
	const int2 GetCameraEnd(const Mode cameraMode) const;

	const float2& GetParallaxCameraInternalSize() const;

	// Rewind data.
	void StoreRewindData(CameraRewindData& rewindData);
	void Rewind(CameraRewindData& rewindData);
	void Resume(CameraRewindData& rewindData);


	void OnNotify(const ObjectClass objectClass, const Event event) override;


	Shape* shape_{ nullptr };


private:
	// Readjust if it puts camera view out of bounds.
	void NudgeCameraIntoBounds();
	
	// Zoom handlers.
	void ApplyZoom();

	void FixZoomLevel();


	float2 defaultSize_{ 0.0f, 0.0f };

	float speed_ = 32.0f;
	float zoomLevel_{ 1.0f };
	float minZoomLevel_{ 1.0f - (6.0f / 16.0f) };
	float maxZoomLevel_{ 2.0f };
	float zoomIncrement{ 1.0f / 16.0f };

	float startZoomLevel_{ 1.0f };
	float finalZoomLevel_{ 1.0f };

	float elapsedZoomTime_{ 0.0f };
	float maxZoomTime_{ 0.5f };

	Shape* bounds_{ nullptr };
	GameObject* focus_{ nullptr };
};
