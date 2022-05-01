#pragma once

#define MAX_OBJECT 250

#define MAX_WIN_WIDTH 640

#define MAX_WIN_HEIGHT 480

struct InterfaceObject
{
	DWORD ModelID;
	float Width;
	float Height;
	float X;
	float Y;
	float MaxX;
	float MaxY;
	float ScaleX;
	float ScaleY;
	DWORD EventTick;
	bool OnClick;
	bool OnShow;
	BYTE Attribute;
};

enum ObjectID
{

};

class Interface
{
public:

	Interface();

	~Interface();

	void BindObject(short ObjectID, DWORD ModelID, float Width, float Height, float X, float Y);

	void Init();

	static void LoadImages(HDC Hdc);

	static void Work();

	void DrawIMG(short ObjectID, float PosX, float PosY, float ScaleX, float ScaleY);

	void DrawIMG(short ObjectID, float PosX, float PosY, float u, float v, float ScaleX, float ScaleY);

	bool IsWorkZone(short ObjectID);

	static void DrawSelectServer(HDC Hdc);

	static void DrawSelectCharacter(HDC Hdc);

public:

	InterfaceObject Data[MAX_OBJECT];
};

extern Interface gInterface;