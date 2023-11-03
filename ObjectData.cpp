#include "precomp.h"
#include "ObjectData.h"

#include "Parser.h"


ObjectData::ObjectData(const int& mapTileCount, const int2& mapTileSize)
	: mapTileCount_{ mapTileCount }
	, mapTileSize_{ mapTileSize }
{	}


ObjectData::~ObjectData()
{
	DeleteIdArray();
}


void ObjectData::ConstructData(const Parser& parser, const char* rawLevelData)
{
	DeleteIdArray();
	ResizeIdArray(mapTileCount_);
	BuildData(parser, rawLevelData);
}


const int& ObjectData::GetTileCount() const
{
	return mapTileCount_;
}


const int2& ObjectData::GetMapTileSize() const
{
	return mapTileSize_;
}


const int* ObjectData::GetObjectIds() const
{
	return objectIds_;
}


void ObjectData::DeleteIdArray()
{
	delete[] objectIds_;
	objectIds_ = nullptr;
}


void ObjectData::ResizeIdArray(const int& mapTileCount)
{
	objectIds_ = new int[mapTileCount];
}


void ObjectData::BuildData(const Parser& parser, const char* rawLevelData)
{
	parser.ParseTileData(rawLevelData, mapTileCount_, objectIds_, "ObjectLayer");
}