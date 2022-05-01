#include "stdafx.h"
#include "Interface.h"
#include "MiniMap.h"
#include "Protect.h"
#include "Reconnect.h"
#include "Util.h"
#include "Window.h"

Interface gInterface;

Interface::Interface()
{
	ZeroMemory(this->Data, sizeof(this->Data));
}

Interface::~Interface()
{

}

void Interface::BindObject(short MonsterID, DWORD ModelID, float Width, float Height, float X, float Y)
{
	this->Data[MonsterID].EventTick = 0;

	this->Data[MonsterID].OnClick = false;

	this->Data[MonsterID].OnShow = false;

	this->Data[MonsterID].ModelID = ModelID;

	this->Data[MonsterID].Width = Width;

	this->Data[MonsterID].Height = Height;

	this->Data[MonsterID].X = X;

	this->Data[MonsterID].Y = Y;

	this->Data[MonsterID].MaxX = X + Width;

	this->Data[MonsterID].MaxY = Y + Height;

	this->Data[MonsterID].Attribute = 0;
}

void Interface::DrawIMG(short ObjectID, float PosX, float PosY, float ScaleX, float ScaleY)
{
	if (this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1)
	{
		this->Data[ObjectID].X = PosX;

		this->Data[ObjectID].Y = PosY;

		this->Data[ObjectID].MaxX = PosX + this->Data[ObjectID].Width;

		this->Data[ObjectID].MaxY = PosY + this->Data[ObjectID].Height;
	}

	RenderBitmap(this->Data[ObjectID].ModelID, PosX, PosY, this->Data[ObjectID].Width, this->Data[ObjectID].Height, 0, 0, ScaleX, ScaleY, 1, 1);
}

void Interface::DrawIMG(short ObjectID, float PosX, float PosY, float u, float v, float ScaleX, float ScaleY)
{
	if (this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1)
	{
		this->Data[ObjectID].X = PosX;

		this->Data[ObjectID].Y = PosY;

		this->Data[ObjectID].MaxX = PosX + this->Data[ObjectID].Width;

		this->Data[ObjectID].MaxY = PosY + this->Data[ObjectID].Height;
	}

	RenderBitmap(this->Data[ObjectID].ModelID, PosX, PosY, this->Data[ObjectID].Width, this->Data[ObjectID].Height, u, v, ScaleX, ScaleY, 1, 1);
}

bool Interface::IsWorkZone(short ObjectID)
{
	if ((pCursorX < this->Data[ObjectID].X || pCursorX > this->Data[ObjectID].MaxX) || (pCursorY < this->Data[ObjectID].Y || pCursorY > this->Data[ObjectID].MaxY))
	{
		return false;
	}

	return true;
}

void Interface::Init()
{
	SetCompleteHook(0xE8, 0x00513950, &this->LoadImages);
	SetCompleteHook(0xE8, 0x0051396E, &this->LoadImages);

	SetCompleteHook(0xE8, 0x004BC0EE, &this->Work);

	SetCompleteHook(0xE8, 0x0052698A, &this->DrawSelectServer);

	SetCompleteHook(0xE8, 0x005269A2, &this->DrawSelectCharacter);
}

void Interface::LoadImages(HDC Hdc)
{
	((void(__cdecl*)(HDC Hdc)) 0x00510320)(Hdc);

	gMiniMap.LoadImages();
}

void Interface::Work()
{
	((void(*)())0x004BD2B0)();

	ReconnectMainProc();

	gWindow.ChangeWindowText();
}

void Interface::DrawSelectServer(HDC Hdc)
{
	SetWindowText(g_hWnd, gProtect.m_MainInfo.WindowName);

	((void(__cdecl*)(HDC Hdc)) 0x00521630)(Hdc);
}

void Interface::DrawSelectCharacter(HDC Hdc)
{
	SetWindowText(g_hWnd, gProtect.m_MainInfo.WindowName);

	((void(__cdecl*)(HDC Hdc)) 0x00523B30)(Hdc);
}