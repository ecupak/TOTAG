#pragma once

struct CameraRewindData
{
	float2 position_{ 0.0f, 0.0f };

	float zoomLevel_{ 0.0f };
	float startZoomLevel_{ 0.0f };
	float finalZoomLevel_{ 0.0f };
	float elapsedZoomTime_{ 0.0f };
};