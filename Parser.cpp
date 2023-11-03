#include "precomp.h"
#include "Parser.h"

#include "GameObject.h"


void Parser::ParseLevelData(const char* body, int2& size)
{
	size.x = GetMapData(body, "width");
	size.y = GetMapData(body, "height");
}


// Find the property and return the value (single int per property).
int Parser::GetMapData(const char* body, char* property) const
{
	// Get map data.
	// ... preserve original body.
	char* bodyDouble = new char[strlen(body) + 1];
	strcpy(bodyDouble, body);

	// ... skip these things when searching for tokens.
	char* delimiterSet{ " =:,\"\n" };

	// ... find property and jump to its value.
	char* startCursor{ strstr(bodyDouble, property) };
	startCursor += strlen(property);

	// ... finally, find the value after the property.
	char* token = strtok(startCursor, delimiterSet);
	int value = ConvertCharToInt(token);

	delete[] bodyDouble;

	return value;
}


void Parser::ParseTileData(const char* body, int maxTileId, int tileIds[], char* layerName) const
{
	// First get the offset for the level tiles. If more than 1 tilemap is used in the level editor, 
	// tilemaps after the first have a starting index equal to the previous tilemap's max index + 1.
	int indexOffset = GetMapData(body, "firstgid");

	// Get tile id data.	
	// ... preserve original body.
	char* bodyDouble = new char[strlen(body) + 1];
	strcpy(bodyDouble, body);

	// ... skip these things when searching for tokens.
	char* delimiterSet{ " :,\"\n" };

	// ... signature start of tile data (this is the same for all layers, so we must find layer name first).
	char* dataStart{ "<data encoding=\"csv\">" };

	// ... jump to layer name, and then to the beginning of the start of tile data.
	char* startCursor{ strstr(bodyDouble, layerName) };
	startCursor = strstr(startCursor, dataStart);
	startCursor += strlen(dataStart);

	// ... finally, start reading the numbers as char.
	char* token = strtok(startCursor, delimiterSet);
	int index{ 0 };
	while (token != NULL && index < maxTileId)
	{
		// Store tile id.
		int value = ConvertCharToInt(token);
		tileIds[index] = value - indexOffset;

		// Get next token.
		token = strtok(NULL, delimiterSet);
		++index;
	}

	delete[] bodyDouble;
}


// Convert an array of char's into an int of the expected value ("563" = 563).
int Parser::ConvertCharToInt(char* alpha) const
{
	int value{ 0 };

	for (size_t index{ 0 }, exponent{ strlen(alpha) - 1 }; index < strlen(alpha); ++index, --exponent)
	{
		int num{ alpha[index] - '0' };

		value += static_cast<int>(num * pow(10, exponent));
	}

	return value;
}