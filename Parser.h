#pragma once


class Parser
{
public:	
	Parser() { }


	void ParseLevelData(const char* body, int2& size);

	
	// Find the property and return the value (single int per property).
	int GetMapData(const char* body, char* property) const;


	void ParseTileData(const char* body, int maxTileId, int tileIds[], char* layerName) const;


	// Convert an array of char's into an int of the expected value ("563" = 563).
	int ConvertCharToInt(char* alpha) const;
};

