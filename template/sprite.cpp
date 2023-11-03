// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"

using namespace Tmpl8;

// constructor
Sprite::Sprite( Surface* surface, bool isOwner, unsigned int frameCount ) :
	rows(1),
	columns(frameCount),
	width( surface->width / frameCount ),
	height( surface->height ),
	numFrames( frameCount ),
	currentFrame( 0 ),
	flags( 0 ),
	start( new unsigned int* [frameCount] ),
	surface( surface ),
	isOwner(isOwner)
{
	InitializeStartData();
}


Sprite::Sprite(Surface* surface, bool isOwner, int spriteWidth, int spriteHeight) :
	rows(surface->height / spriteHeight),
	columns(surface->width / spriteWidth),
	width(spriteWidth),
	height(spriteHeight),
	numFrames(rows * columns),
	currentFrame(0),
	flags(0),
	start(new unsigned int* [rows * columns]),
	surface(surface),
	isOwner(isOwner)
{
	InitializeStartData();
}

// destructor
Sprite::~Sprite()
{
	if (isOwner)
	{
		delete surface;
	}
	
	for (unsigned int i = 0; i < numFrames; i++) 
	{
		delete start[i];
	}

	delete start;
}

// Have the sprite flip its horizontal facing. isFlipped is false when facing left.
void Sprite::SetFacing(bool isFlipped)
{
	isFlipped_ = isFlipped;	
}

// Let the sprite advance to the next frame itself.
void Sprite::AdvanceToNextFrame()
{
	SetFrame((currentFrame + 1) % numFrames);
}

// Current frame creates a row and column index.
void Sprite::SetFrame(unsigned int i)
{ 
	currentFrame = i;
	currentRow = GetRow(i);
	currentColumn = GetColumn(i);
}

// Draw sprite to target surface
void Sprite::Draw( Surface* target, int x, int y)
{
	// If none of the sprite is within bounds, exit.
	if (x < -width || x > ( target->width + width )) return;
	if (y < -height || y > ( target->height + height )) return;

	int x1 = x, x2 = x + width;
	int y1 = y, y2 = y + height;

	int xStartOffset{ 0 };
	int xEndOffset{ 0 };
	// Starting position (top-left of subsprite image).
	uint* src = GetBuffer() + (currentColumn * width) + (currentRow * height * columns * width);

	// If some of the sprite is outside bounds, move starting points to within bounds.
	if (x1 < 0)
	{
		xStartOffset = -x1;
		x1 = 0;
	}

	if (x2 > target->width)
	{
		xEndOffset = x2 - target->width;
		x2 = target->width;
	}

	if (y1 < 0)
	{
		// (width [of a sprite] * columns) is same as width of the screen.
		src += -y1 * (width * columns);
		y1 = 0;
	}

	if (y2 > target->height)
	{
		y2 = target->height;
	}

	// Get destination (screen) pointer. This is to the starting (0, 0) pixel.
	uint* dest = target->pixels;

	// If some of the sprite is still visible after moving starting points, draw it.
	if (x2 > x1 && y2 > y1)
	{
		// Offset for destination.
		unsigned int addr = x1 + (y1 * target->width);

		// The range to draw.
		const int w = x2 - x1;
		const int h = y2 - y1;

		// j is row/y. We draw row by row, starting at the top.
		int xs;
		for (int j = 0; j < h; ++j)
		{
			// Use stored positions of 1st non-0 pixel on each row to speed up drawing process:
			
			// ... local starting row of sprite
			const int line = j + (y1 - y); 
			
			// ... local non-zero pixel position + world transform
			const int lsx = start[currentFrame][line] + x; 

			// ... adjusted to the initial value of our range (0 ... width - 1).
			xs = (lsx > x1) ? lsx - x1 : 0;
			
			// i is column/x. We draw horizontally until the end of row/w is reached.
			if (isFlipped_)
			{
				for (int i = 0; i < w; ++i)
				{
					//const uint c1 = *(src + i);
					const uint c1 = *(src + i + xEndOffset);

					// If pixel value is more than 0 draw it.
					if (c1 & 0xffffff)
					{
						*(dest + addr + (w - 1) - i) = c1;
					}
				}
			}
			else
			{
				for (int i = xs; i < w; ++i)
				{
					const uint c1 = *(src + i + xStartOffset);

					// If pixel value is more than 0 draw it.
					if (c1 & 0xffffff)
					{
						*(dest + addr + i) = c1;
					}
				}
			}

			// Advance to next row.
			addr += target->width;
			src += width * columns;
		}
	}
}


// draw scaled sprite
void Sprite::DrawScaled( int x1, int y1, int w, int h, Surface* target )
{
	if (width == 0 || height == 0) return;
	for (int x = 0; x < w; x++) for (int y = 0; y < h; y++)
	{
		int u = (int)((float)x * ((float)width / (float)w));
		int v = (int)((float)y * ((float)height / (float)h));
		uint color = GetBuffer()[u + v * width * numFrames];
		target->pixels[x1 + x + ((y1 + y) * target->width)] = color;
	}
}


// Draw scaled, but with the ability to grab only a section of the source screen if desired.
void Sprite::DrawSectionScaled(int srcX, int srcY, int srcWidth, int srcHeight,
	int destX, int destY, int destWidth, int destHeight, Surface* dest)
{
	// Early out.
	if (width == 0 || height == 0) return;


	// Precalculate some things.
	int pitch{ columns * width };


	float2 scaleRatio{
		static_cast<float>(1.0f * srcWidth / destWidth),
		static_cast<float>(1.0f * srcHeight / destHeight)
	};


	int2 end{
		destWidth - destX,
		destHeight - destY
	};
	
	uint* dpix{ dest->pixels + destX + (destY * dest->width) };
	uint dOffset{ 0 };

	uint* spix{ GetBuffer() };
	uint sOffset{ 0 };

	// Do the thing.
	for (int y{ 0 }; y < end.y; ++y)
	{
		int v{ static_cast<int>((srcY + y) * (scaleRatio.y)) };

		sOffset = v * pitch;
		dOffset = y * dest->width;

		for (int x{ 0 }; x < end.x; ++x)
		{
			int u{ static_cast<int>((srcX + x) * (scaleRatio.x)) };			

			dpix[x + dOffset] = spix[u + sOffset];
		}		
	}
}


// Prepare sprite outline data for faster rendering.
void Sprite::InitializeStartData()
{
	for (unsigned int f = 0; f < numFrames; ++f)
	{
		start[f] = new unsigned int[height];
		for (int y = 0; y < height; ++y)
		{
			int xOffset = (GetColumn(f) * width);
			int yOffset = (GetRow(f) * height * width * columns);
			int lineOffset = (y * width * columns);
			// Get pixel at start of line.	
			uint* addr = GetBuffer() + xOffset + yOffset + lineOffset;
			
			// Store first position in line that has a non-zero pixel value.
			// If none found, set width as the position (sets as end + 1, so not valid?)
			start[f][y] = width;
			for (int x = 0; x < width; ++x) if (addr[x])
			{
				start[f][y] = x;
				break;
			}
		}
	}
}

// Get row of sprite frame.
int Sprite::GetRow(int index)
{
	return static_cast<int>(index / columns);	
}

// Get column of sprite frame.
int Sprite::GetColumn(int index)
{
	return index % columns;
}


// Get buffer at the start of current sprite frame.
uint* Sprite::GetCurrentFrameBuffer()
{
	return surface->pixels +  (currentColumn * width) + (currentRow * height * columns * width);
}