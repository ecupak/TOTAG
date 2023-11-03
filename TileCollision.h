#pragma once

enum TileCollisionID
{
	Nothing =0b0000'0000,
	Obstacle = 0b0000'0001,
	Ladder = 0b0000'0010,
	ZiplineL = 0b0000'0100,
	ZiplineR = 0b0000'1000,
	Fan = 0b0001'0000,
	Spike = 0b0010'0000,
	Trampoline = 0b0100'0000,
	Ledge = 0b1000'0000,
	Cliff = 0b1'0000'0000,
};