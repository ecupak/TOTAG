#include "precomp.h"
#include "Camera.h"

#include "Shape.h"

#include "Event.h"
#include "GameObject.h"
#include "CameraRewindData.h"

#include <iostream>
#include <iomanip>


Camera::Camera()
	: shape_{ new Shape{} }
	, bounds_{ new Shape{} }
{	}


void Camera::Initialize(Surface* stagingScreen)
{
	// Set shape of the camera (same size as staging screen).
	// This is the view of the world. Anything in this view will be rendered.
	shape_->size_ = float2{ (1.0f * stagingScreen->width) + (2.0f * margin_.x), (1.0f * stagingScreen->height) + (2.0f * margin_.y) };
	shape_->start_ = float2{ 0.0f, 0.0f };

	// Store default size, zoom x1. The staging screen is the size of zoom x2 so it will have enough space when drawn at that zoom level.
	defaultSize_ = { 0.5f * stagingScreen->width, 0.5f * stagingScreen->height };
}


Camera::~Camera()
{
	delete shape_;
	delete bounds_;
}


void Camera::SetFocus(GameObject* focus)
{
	focus_ = focus;

	SetZoomLevel(1.0f);
}


void Camera::SetLevelBounds(const int2& mapTileSize)
{
	// Set bounding box for camera movement. It collides with the /inside/ of an object (the level).
	bounds_->size_ = float2{ static_cast<float>(mapTileSize.x * TILE_WIDTH), static_cast<float>(mapTileSize.y * TILE_HEIGHT) };
	bounds_->SetPosition(float2{ 0.0f, 0.0f });

	NudgeCameraIntoBounds();
}


void Camera::Update(const float deltaTime)
{
	shape_->SetCenter(focus_->shape_->center_);

	if (zoomLevel_ != finalZoomLevel_)
	{
		elapsedZoomTime_ += deltaTime;

		float progress{ clamp(elapsedZoomTime_ / maxZoomTime_, 0.0f, 1.0f) };

		zoomLevel_ = ec::Lerpf(startZoomLevel_, finalZoomLevel_, progress);

		ApplyZoom();
	}

	NudgeCameraIntoBounds();
}


void Camera::ChangeZoomLevel(const float deltaZoomLevel)
{
	startZoomLevel_ = zoomLevel_;
	finalZoomLevel_ = startZoomLevel_ + deltaZoomLevel;
	elapsedZoomTime_ = 0.0f;

	ApplyZoom();
}


void Camera::SetZoomLevel(const float newZoomLevel)
{
	startZoomLevel_ = zoomLevel_;
	finalZoomLevel_ = newZoomLevel;
	elapsedZoomTime_ = 0.0f;

	ApplyZoom();
}


const float Camera::GetZoomLevel() const
{
	return zoomLevel_;
}


const int2 Camera::GetCameraStart(const Mode cameraMode) const
{
	if (cameraMode == Mode::Normal)
	{
		return {
			static_cast<int>(shape_->start_.x + margin_.x),
			static_cast<int>(shape_->start_.y + margin_.y)
		};
	}
	else
	{
		return {
			static_cast<int>(shape_->center_.x - (defaultSize_.x * 0.5f)),
			static_cast<int>(shape_->center_.y - (defaultSize_.y * 0.5f))
		};
	}
}


const int2 Camera::GetCameraEnd(const Mode cameraMode) const
{
	if (cameraMode == Mode::Normal)
	{
		return {
			static_cast<int>(shape_->end_.x - margin_.x) + 1,
			static_cast<int>(shape_->end_.y - margin_.y) + 1
		};
	}
	else
	{
		return {
			static_cast<int>(shape_->center_.x + (defaultSize_.x * 0.5f)),
			static_cast<int>(shape_->center_.y + (defaultSize_.y * 0.5f))
		};
	}
}


const float2& Camera::GetParallaxCameraInternalSize() const
{
	return defaultSize_;
}


void Camera::StoreRewindData(CameraRewindData& rewindData)
{
	rewindData.elapsedZoomTime_ = elapsedZoomTime_;
	rewindData.finalZoomLevel_ = finalZoomLevel_;
	rewindData.startZoomLevel_ = startZoomLevel_;
	rewindData.zoomLevel_ = zoomLevel_;
	rewindData.position_ = static_cast<float2>(shape_->start_);
}


void Camera::Rewind(CameraRewindData& rewindData)
{
	zoomLevel_ = rewindData.zoomLevel_;
	
	ApplyZoom();
	
	shape_->SetPosition(rewindData.position_);
}


void Camera::Resume(CameraRewindData& rewindData)
{
	startZoomLevel_ = rewindData.startZoomLevel_;
	finalZoomLevel_ = rewindData.finalZoomLevel_;
	elapsedZoomTime_ = rewindData.elapsedZoomTime_;
}


void Camera::OnNotify(const ObjectClass objectClass, const Event event)
{
	if (event == Event::OnTouch)
	{
		SetZoomLevel(1.0f + zoomIncrement * 8.0f);
	}
	else if (event == Event::OnRelease)
	{
		SetZoomLevel(1.0f);
	}
}


// Private methods.


// [Credit] Sprite::Draw()
void Camera::NudgeCameraIntoBounds()
{
	if ((shape_->start_.x + margin_.x) < 0.0f)
	{
		shape_->start_.x = 0.0f - margin_.x;
		shape_->UpdateCenter();
	}

	if ((shape_->start_.y + margin_.y) < 0.0f)
	{
		shape_->start_.y = 0.0f - margin_.y;
		shape_->UpdateCenter();
	}

	if ((shape_->end_.x - margin_.x) > bounds_->end_.x)
	{
		shape_->start_.x = bounds_->end_.x - shape_->size_.x + 1 + margin_.x;
		shape_->UpdateCenter();
	}

	if ((shape_->end_.y - margin_.y) > bounds_->end_.y)
	{
		shape_->start_.y = bounds_->end_.y - shape_->size_.y + 1 + margin_.y;
		shape_->UpdateCenter();
	}
}


// Adjust size of camera based on zoomlevel.

void Camera::ApplyZoom()
{
	FixZoomLevel();

	shape_->size_ = float2{
		(defaultSize_.x * zoomLevel_) + (2 * margin_.x),
		(defaultSize_.y * zoomLevel_) + (2 * margin_.y)
	};
}


// Makes sure zoom stays within valid bounds.
void Camera::FixZoomLevel()
{
	zoomLevel_ = clamp(zoomLevel_, minZoomLevel_, maxZoomLevel_);
}