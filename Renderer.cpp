#include "precomp.h"
#include "Renderer.h"

#include "Camera.h"
#include "LayerData.h"
#include "GameObject.h"
#include "Scorekeeper.h"
#include "Player.h"

#include "Shape.h"

#include <iostream>
#include <iomanip>


Renderer::Renderer(Surface* stagingSurface, Surface* hudSurface, const Camera& camera, GameObject* const activeList[], Sprite& tileset, const LayerData& layerData)
	: stage_{ stagingSurface }
	, overlay_{ hudSurface }
	, camera_{ camera }
	, activeList_{ activeList }
	, layerData_{ layerData }
	, tileset_{ tileset }
{	}


void Renderer::RenderView(const int activeCount)
{
	// Note that camera bounds are extended beyond the screen by margin.
	// This was to allow enemies and objects just offscreen to be "touched" and get
	// to update. Possibly will remove as collision will change.
	float2 cameraStart{
		camera_.shape_->start_.x + camera_.margin_.x,
		camera_.shape_->start_.y + camera_.margin_.y
	};

	float2 cameraEnd{
		camera_.shape_->end_.x - camera_.margin_.x + 1,
		camera_.shape_->end_.y - camera_.margin_.y + 1
	};

	// Get the top-left tile being overlapped. Find it's world position/origin. Now in all subsequent loops, advance by tilesize until beyond the camera's x/y end.
	int2 initialTilePosition{ static_cast<int>(cameraStart.x / TILE_WIDTH) * TILE_WIDTH, static_cast<int>(cameraStart.y / TILE_HEIGHT) * TILE_HEIGHT };

	// Draw furthest away to closest.
	
	// ... Far/Mid/Near BG
	DrawParallaxLayer(initialTilePosition, cameraStart, cameraEnd, Layer::FarBG);
	DrawParallaxLayer(initialTilePosition, cameraStart, cameraEnd, Layer::MidBG);
	DrawParallaxLayer(initialTilePosition, cameraStart, cameraEnd, Layer::NearBG);

	// ... Low/Mid doodad.
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::LowDoodad);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::MidDoodad);

	// ... Ropes, ziplines, trampolines.
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Ladder);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Zipline);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Trampoline);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Obstacle);

	// ... Moving objects, collectables, enemies, player.
	DrawDynamicObjects(initialTilePosition, cameraStart, cameraEnd, activeCount);

	// ... Ledges, spikes, high doodad.
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Ledge);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::Spike);
	DrawLayer(initialTilePosition, cameraStart, cameraEnd, Layer::HighDoodad);
}


void Renderer::RenderHUD(const Scorekeeper* scorekeeper, const Player* player)
{
	int scaling{ 6 };

	PrintValue("Score", scorekeeper->GetScore(Scorekeeper::Score::Both), 10, 10, scaling);
	PrintValue("HP", player->GetCurrentHP(), 10, 10 + 20 * scaling, scaling);
	PrintValue("Rocks", player->GetRockInventory(), 10, 10 + 40 * scaling, scaling);
}


void Renderer::PrintValue(const char* label, const int value, const int x, const int y, const int scaling)
{
	char buffer[50]{};
	ec::GetCharFromInt(value, buffer);
	overlay_->Print(label, x, y, 0x000F0F, scaling);
	overlay_->Print(buffer, x, y + (6 * scaling), 0x000F0F, scaling);
}


// PRIVATE METHODS

void Renderer::DrawParallaxLayer(const int2& start, const float2& cameraStart, const float2& cameraEnd, const Layer layer)
{
	// Parallax.y is anchored to the bottom of the map.
	int mapHeight{ layerData_.GetTileSize().y * TILE_HEIGHT };
	float anchorPoint{ (mapHeight - 1) - (camera_.shape_->size_.y - (2.0f * camera_.margin_.y)) };
	float distanceAboveAnchorPoint{ anchorPoint - cameraStart.y };

	// Find position of parallax camera.
	const float2& parallaxSpeed{ layerData_.GetParallaxSpeed(layer) };

	float2 parallaxCameraStart{
		cameraStart.x * parallaxSpeed.x,
		anchorPoint - (distanceAboveAnchorPoint * parallaxSpeed.y)
	};

	float2 parallaxCameraEnd{
		parallaxCameraStart.x + (camera_.shape_->size_.x - (2.0f * camera_.margin_.x)),
		parallaxCameraStart.y + (camera_.shape_->size_.y - (2.0f * camera_.margin_.y))
	};

	// Precalculate the first tile-in-view's origin.
	int2 initialParallaxTilePosition{ static_cast<int>(parallaxCameraStart.x / TILE_WIDTH) * TILE_WIDTH, static_cast<int>(parallaxCameraStart.y / TILE_HEIGHT) * TILE_HEIGHT };

	DrawLayer(initialParallaxTilePosition, parallaxCameraStart, parallaxCameraEnd, layer);
}


void Renderer::DrawLayer(const int2& start, const float2& cameraStart, const float2& cameraEnd, const Layer layer)
{
	// Find column/row of start (row/y will be unused). Advance layerid array to the starting row.
	int2 arrayPosStart{ start.x / TILE_WIDTH, start.y / TILE_HEIGHT };
	const int* layerIds{ layerData_.GetLayerIds(layer) + (arrayPosStart.y * layerData_.GetTileSize().x) };

	// Loop through what the camera sees. Components X and Y of start are already at the world position of the tile's origin (top-left corner).
	int2 worldPos{ 0, 0 };
	int x{ 0 };
	for (worldPos.y = start.y; worldPos.y < cameraEnd.y; worldPos.y += TILE_HEIGHT)
	{
		for (worldPos.x = start.x, x = arrayPosStart.x; worldPos.x < cameraEnd.x; worldPos.x += TILE_WIDTH, ++x)
		{	
			// Convert to screen position so the tiles can be drawn relative to camera.
			int2 screenPos{ 
				static_cast<int>(floorf(worldPos.x - cameraStart.x)), 
				static_cast<int>(floorf(worldPos.y - cameraStart.y)) 
			};

			DrawTile(screenPos, layerIds[x]);
		}

		layerIds += layerData_.GetTileSize().x;
	}
}


void Renderer::DrawTile(const int2& screenPos, const int tileId)
{
	if (tileId > 0)
	{
		tileset_.SetFrame(tileId);
		tileset_.Draw(stage_, screenPos.x, screenPos.y);

		/*for (int i = 0; i < tileset_.Frames(); ++i)
		{
			for (int y = 0; y < TILE_HEIGHT; ++y)
			{
				if (tileset_.start[i][y] > TILE_WIDTH)
				{
					int x = 3;
				}
			}
		}*/

		//stage_->Box(screenPos.x, screenPos.y, screenPos.x + 15, screenPos.y + 15, 0);
	}
}


// Render enemies, coins, and dynamic environmental objects. The object CM already has marked what should be drawn as 'onstage'.
void Renderer::DrawDynamicObjects(const int2& start, const float2& cameraStart, const float2& cameraEnd, const int activeCount)
{
	int2 cameraFloored{
		static_cast<int>(floorf(camera_.shape_->start_.x) + camera_.margin_.x),
		static_cast<int>(floorf(camera_.shape_->start_.y) + camera_.margin_.y)
	};

	for (int index{ 0 }; index < activeCount; ++index)
	{
		GameObject* const object{ activeList_[index] };

		if (object->isOnstage_)
		{	/*				
			int2 screenPos{	// Offset by the camera's position.
				static_cast<int>(object->shape_->start_.x) - cameraFloored.x,
				static_cast<int>(object->shape_->start_.y) - cameraFloored.y
			};
			*/
			
			int2 screenPos{	// Offset by the camera's position.
				static_cast<int>(object->shape_->start_.x - cameraStart.x),
				static_cast<int>(object->shape_->start_.y - cameraStart.y)
			};
					
			/*
			if (object->objectLayer_ & ObjectId::Player)
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(8);
				std::cout << "fPos: " << object->shape_->start_.x
					<< " - fCam: " << cameraStart.x
					<< " = " << object->shape_->start_.x - cameraStart.x
					<< '\n'
					<< "iPos: " << static_cast<int>(object->shape_->start_.x)
					<< " - floorCam: " << cameraFloored.x
					<< " = (int)" << screenPos.x << '\n' << '\n';
			}
			*/

			object->Draw(stage_, screenPos.x, screenPos.y);
		}
	}
}