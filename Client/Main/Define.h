#pragma once

#define MAX_MAIN_VIEWPORT 400

#define Color3f(r, b, g)	((g << 16) + (b << 8) + r)
#define Color4f(r, b, g, o)	((o << 24) + (g << 16) + (b << 8) + r)

#define MAX_ITEM_SECTION 16
#define MAX_ITEM_TYPE 32
#define MAX_ITEM (MAX_ITEM_SECTION*MAX_ITEM_TYPE)
#define ITEM_BASE_MODEL 400
#define GET_ITEM(x,y) (((x)*MAX_ITEM_TYPE)+(y))
#define GET_ITEM_MODEL(x,y) ((((x)*MAX_ITEM_TYPE)+(y))+ITEM_BASE_MODEL)

struct VAngle
{
	float X;
	float Y;
	float Z;
};

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_EXILE = 5,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16
};