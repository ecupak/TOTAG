// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

// common.h is to be included in host and device code and stores
// global settings and defines.

// tile data
#define TILE_WIDTH			16
#define TILE_HEIGHT			16

// default screen resolution
#define TILES_PER_ROW		12
#define TILES_PER_COLUMN	7

#define SCALING_FACTOR		8
#define SCRWIDTH			TILES_PER_ROW * TILE_WIDTH  * SCALING_FACTOR
#define SCRHEIGHT			TILES_PER_COLUMN * TILE_HEIGHT * SCALING_FACTOR
// #define FULLSCREEN

// constants
#define PI			3.14159265358979323846264f
#define INVPI		0.31830988618379067153777f
#define INV2PI		0.15915494309189533576888f
#define TWOPI		6.28318530717958647692528f
#define SQRT_PI_INV	0.56418958355f
#define LARGE_FLOAT	1e34f