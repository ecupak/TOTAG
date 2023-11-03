// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

namespace Tmpl8
{
	// basic sprite class
	class Sprite
	{
	public:
		// structors
		Sprite( Surface* surface, bool isOwner, unsigned int frameCount );
		Sprite(Surface* surface, bool isOwner, int spriteWidth, int spriteHeight);
		~Sprite();
		// methods
		void Draw( Surface* target, int x, int y);
		void DrawScaled( int x, int y, int width, int height, Surface* target );
		void DrawSectionScaled(int srcX1, int srcY1, int srcWidth, int srcHeight,
			int destX1, int destY1, int destWidth, int destHeight, Surface* dest);
		void SetFlags( unsigned int f ) { flags = f; }
		void SetFacing( bool isFlipped );
		void AdvanceToNextFrame();
		void SetFrame(unsigned int i);
		const int GetFrame() const { return currentFrame; }
		unsigned int GetFlags() const { return flags; }
		int GetWidth() { return width; }
		int GetHeight() { return height; }
		int GetRows() { return rows; }
		int GetColumns() { return columns; }
		uint* GetBuffer() { return surface->pixels; }
		uint* GetCurrentFrameBuffer();
		unsigned int Frames() { return numFrames; }
		Surface* GetSurface() { return surface; }
		void InitializeStartData();
		const bool IsFlipped() const { return isFlipped_; }
	private:
		int GetRow(int index);
		int GetColumn(int index);
		// attributes
		int rows, columns;
		int width, height;		
		unsigned int numFrames;
		unsigned int currentFrame;
		unsigned int flags;
		bool isOwner;
		Surface* surface;
		unsigned int** start;
		int currentRow{ 0 }, currentColumn{ 0 };
		bool isFlipped_{ false };
	};
}