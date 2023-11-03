#pragma once

class Parser;

enum class Layer
{
	HighDoodad,
	MidDoodad,
	LowDoodad,

	Spike,
	Zipline,
	Trampoline,
	Ladder,

	Ledge,
	Obstacle,

	NearBG,
	MidBG,
	FarBG
};


class LayerData
{
public:
	LayerData(Sprite& tilesetSprite, const int& mapTileCount, const int2& mapTileSize);
	~LayerData();

	void ConstructData(const Parser& parser, const char* rawLevelData);

	const float2& GetParallaxSpeed(const Layer layer) const;

	const int& GetTileCount() const;

	const int2& GetTileSize() const;

	const int* GetLayerIds(const Layer layer) const;

	const int GetLayerIdAt(const Layer layer, const int index);


private:
	void DeleteTileIdArrays();
	void ResizeTileIdArrays();
	void BuildLayerData(const Parser& parser, const char* rawLevelData);

	Sprite& tilesetSprite_;

	const int& mapTileCount_;
	const int2& mapTileSize_;

	int* obstacleIds_{ nullptr };
	int* ledgeIds_{ nullptr };
	int* spikeIds_{ nullptr };
	int* ladderIds_{ nullptr };
	int* ziplineIds_{ nullptr };
	int* trampolineIds_{ nullptr };
	int* highDoodadIds_{ nullptr };
	int* midDoodadIds_{ nullptr };
	int* lowDoodadIds_{ nullptr };
	int* nearBackgroundIds_{ nullptr };
	int* midBackgroundIds_{ nullptr };
	int* farBackgroundIds_{ nullptr };

	const float2 parallaxSpeeds_[12]{
		{1.0f, 1.0f}, // HighDoodad,
		{1.0f, 1.0f}, // MidDoodad,
		{1.0f, 1.0f}, // LowDoodad,

		{1.0f, 1.0f}, // Spike,
		{1.0f, 1.0f}, // Zipline,
		{1.0f, 1.0f}, // Trampoline,
		{1.0f, 1.0f}, // Ladder,

		{1.0f, 1.0f}, // Ledge,
		{1.0f, 1.0f}, // Obstacle,

		{0.5f, 0.5f}, // NearBG,
		{0.1f, 0.002f}, // MidBG,
		{0.0f, 0.0f}, // FarBG
	};
};