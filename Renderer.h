#pragma once

class GameObject;
class Camera;
class LayerData;
class Scorekeeper;
class Player;

enum class Layer;


class Renderer
{
public:
	Renderer(Surface* stagingSurface, Surface* hudSurface, const Camera& camera, GameObject* const activeList[], Sprite& tileset, const LayerData& layerData);

	void RenderView(const int activeCount);

	void RenderHUD(const Scorekeeper* scorekeeper, const Player* player);


private:
	void DrawParallaxLayer(const int2& cameraStart, const Layer layer);
	void DrawLayer(const int2& start, const int2& cameraStart, const int2& cameraEnd, const Layer layer);
	
	void DrawTile(const int2& screenPos, const int tileId);

	void DrawDynamicObjects(const int2& start, const int2& cameraStart, const int2& cameraEnd, const int activeCount);

	void PrintValue(const char* label, const int value, const int x, const int y, const int scaling);


	// Ordered by initialization list.
	GameObject* const (*activeList_) { nullptr };
	Surface* stage_{ nullptr };
	Surface* overlay_{ nullptr };
	const Camera& camera_;
	const LayerData& layerData_;
	Sprite& tileset_;
};