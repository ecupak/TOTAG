#pragma once

class Parser;


class ObjectData
{
public:
	ObjectData(const int& mapTileCount, const int2& mapTileSize);
	~ObjectData();


	void ConstructData(const Parser& parser, const char* rawLevelData);

	const int& GetTileCount() const;

	const int2& GetMapTileSize() const;

	const int* GetObjectIds() const;


private:
	void DeleteIdArray();
	void ResizeIdArray(const int& mapTileCount);
	void BuildData(const Parser& parser, const char* rawLevelData);


	const int& mapTileCount_;
	const int2& mapTileSize_;

	int* objectIds_{ nullptr };
};

