#include "precomp.h"
#include "LayerData.h"

#include "Parser.h"


LayerData::LayerData(Sprite& tilesetSprite, const int& mapTileCount, const int2& mapTileSize)
	: tilesetSprite_{ tilesetSprite }
	, mapTileCount_{ mapTileCount }
	, mapTileSize_{ mapTileSize }
{	}


LayerData::~LayerData()
{
	DeleteTileIdArrays();
}


void LayerData::ConstructData(const Parser& parser, const char* rawLevelData)
{
	DeleteTileIdArrays();
	ResizeTileIdArrays();
	BuildLayerData(parser, rawLevelData);
}


// Erases any previous data.
void LayerData::DeleteTileIdArrays()
{
	delete[] obstacleIds_;
	obstacleIds_ = nullptr;

	delete[] ledgeIds_;
	ledgeIds_ = nullptr;

	delete[] spikeIds_;
	spikeIds_ = nullptr;

	delete[] ladderIds_;
	ladderIds_ = nullptr;

	delete[] ziplineIds_;
	ziplineIds_ = nullptr;

	delete[] trampolineIds_;
	trampolineIds_ = nullptr;

	delete[] highDoodadIds_;
	highDoodadIds_ = nullptr;

	delete[] midDoodadIds_;
	midDoodadIds_ = nullptr;

	delete[] lowDoodadIds_;
	lowDoodadIds_ = nullptr;

	delete[] nearBackgroundIds_;
	nearBackgroundIds_ = nullptr;

	delete[] midBackgroundIds_;
	midBackgroundIds_ = nullptr;

	delete[] farBackgroundIds_;
	farBackgroundIds_ = nullptr;
}


// Set all helper arrays to size of map.
void LayerData::ResizeTileIdArrays()
{
	obstacleIds_ = new int[mapTileCount_];
	ledgeIds_ = new int[mapTileCount_];
	spikeIds_ = new int[mapTileCount_];
	ladderIds_ = new int[mapTileCount_];
	ziplineIds_ = new int[mapTileCount_];
	trampolineIds_ = new int[mapTileCount_];
	highDoodadIds_ = new int[mapTileCount_];
	midDoodadIds_ = new int[mapTileCount_];
	lowDoodadIds_ = new int[mapTileCount_];
	nearBackgroundIds_ = new int[mapTileCount_];
	midBackgroundIds_ = new int[mapTileCount_];
	farBackgroundIds_ = new int[mapTileCount_];
}


// Store each tile's tilset id in an array that represents each layer.
void LayerData::BuildLayerData(const Parser& parser, const char* rawLevelData)
{
	parser.ParseTileData(rawLevelData, mapTileCount_, obstacleIds_, "ObstacleLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, ledgeIds_, "LedgeLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, spikeIds_, "SpikeLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, ladderIds_, "RopeLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, ziplineIds_, "ZiplineLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, trampolineIds_, "TrampolineLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, highDoodadIds_, "HighDoodadLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, midDoodadIds_, "MidDoodadLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, lowDoodadIds_, "LowDoodadLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, nearBackgroundIds_, "NearBGLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, midBackgroundIds_, "MidBGLayer");
	parser.ParseTileData(rawLevelData, mapTileCount_, farBackgroundIds_, "FarBGLayer");
}


const float2& LayerData::GetParallaxSpeed(const Layer layer) const
{
	return parallaxSpeeds_[static_cast<int>(layer)];
}


const int& LayerData::GetTileCount() const
{
	return mapTileCount_;
}


const int2& LayerData::GetTileSize() const
{
	return mapTileSize_;
}


const int* LayerData::GetLayerIds(const Layer layer) const
{
	switch (layer)
	{
	case Layer::HighDoodad:
		return highDoodadIds_;
	case Layer::MidDoodad:
		return midDoodadIds_;
	case Layer::LowDoodad:
		return lowDoodadIds_;
	case Layer::Spike:
		return spikeIds_;
	case Layer::Zipline:
		return ziplineIds_;
	case Layer::Trampoline:
		return trampolineIds_;
	case Layer::Ladder:
		return ladderIds_;
	case Layer::Ledge:
		return ledgeIds_;
	case Layer::Obstacle:
		return obstacleIds_;
	case Layer::NearBG:
		return nearBackgroundIds_;
	case Layer::MidBG:
		return midBackgroundIds_;
	case Layer::FarBG:
		return farBackgroundIds_;
	default:
		return nullptr;
	}
}


const int LayerData::GetLayerIdAt(const Layer layer, const int index)
{
	return GetLayerIds(layer)[index];
}