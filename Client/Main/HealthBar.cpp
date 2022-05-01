#include "stdafx.h"
#include "HealthBar.h"
#include "Util.h"

CHealthBar gHealthBar;

CHealthBar::CHealthBar()
{
	this->ClearHealthBar();

	this->InitHealthBar();
}

CHealthBar::~CHealthBar()
{

}

void CHealthBar::ClearHealthBar()
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		this->m_MonsterHealthBar[n].index = 0xFFFF;

		this->m_MonsterHealthBar[n].type = 0;

		this->m_MonsterHealthBar[n].rateHP = 0;
	}
}

void CHealthBar::InsertHealthBar(WORD index, BYTE type, BYTE HP)
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (this->m_MonsterHealthBar[n].index == 0xFFFF)
		{
			this->m_MonsterHealthBar[n].index = index;

			this->m_MonsterHealthBar[n].type = type;

			this->m_MonsterHealthBar[n].rateHP = HP;

			return;
		}
	}
}

MONSTER_HEALTH_BAR* CHealthBar::GetHealthBar(WORD index, BYTE type)
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (this->m_MonsterHealthBar[n].index != 0xFFFF)
		{
			if (this->m_MonsterHealthBar[n].index == index && this->m_MonsterHealthBar[n].type == type)
			{
				return &this->m_MonsterHealthBar[n];
			}
		}
	}

	return 0;
}

void CHealthBar::InitHealthBar()
{
	SetCompleteHook(0xE8, 0x004BC0AE, &this->DrawHealthBar);

	SetCompleteHook(0xE9, 0x004CB7AD, &this->DrawPointingHealthBar);
}

void CHealthBar::DrawHealthBar()
{
	((void(*)())0x004BCA20)();

	int PosX, PosY;

	float LifeBarWidth = 70.0f;

	char LifeDisplay[64];

	VAngle Angle;

	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		DWORD ViewportAddress = *(DWORD*)(0x07ABF5D0) + (n * 916);

		if (!ViewportAddress)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		MONSTER_HEALTH_BAR* lpHealthBar = gHealthBar.GetHealthBar(*(WORD*)(ViewportAddress + 0x1DC), *(BYTE*)(ViewportAddress + 0x84));

		if (lpHealthBar == 0)
		{
			continue;
		}

		if (*(WORD*)(ViewportAddress + 0x2EB) == 200) //Soccer Ball
		{
			continue;
		}

		char* MonsterName = (char*)(ViewportAddress + 0x1C1);

		Angle.X = *(float*)(ViewportAddress + 0x10);

		Angle.Y = *(float*)(ViewportAddress + 0x14);

		Angle.Z = *(float*)(ViewportAddress + 0x12C) + *(float*)(ViewportAddress + 0x18) + 100.0f;

		GetPosFromAngle(&Angle, &PosX, &PosY);

		PosX -= (int)floor(LifeBarWidth / (double)2.0);

		if (((int)pCursorX >= PosX) && ((float)pCursorX <= (float)PosX + LifeBarWidth) && ((int)pCursorY >= PosY - 2) && ((int)pCursorY < PosY + 6))
		{
			wsprintf(LifeDisplay, "%s: %d%%", MonsterName, lpHealthBar->rateHP);

			SetBackgroundTextColor = Color4f(0, 0, 0, 0);

			SetTextColor = Color4f(255, 255, 255, 255);

			RenderText(PosX, PosY - 10, LifeDisplay, 0, 0, 0);
		}

		EnableAlphaTest(true);

		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

		RenderColor((float)PosX, (float)PosY, LifeBarWidth, 6.0f);

		EnableAlphaBlend();

		glColor3f(0.64f, 0.29f, 0.64f);

		RenderColor((float)PosX + 2, (float)PosY + 2, ((LifeBarWidth - 4) * lpHealthBar->rateHP) / 100, 2.0f);

		DisableAlphaBlend();

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

__declspec(naked) void CHealthBar::DrawPointingHealthBar()
{
	static DWORD jmpBack = 0x004CB9D5;
	static char LifeDisplay[64];
	static DWORD ViewportAddress;
	static MONSTER_HEALTH_BAR* lpHealthBar;
	static char* MonsterName;
	static float LifeBarWidth;
	static float PosX, PosY;

	_asm
	{
		Pushad;
	}

	ViewportAddress = pViewportAddress + (*(DWORD*)(0x00559C50) * 916);

	if (*(WORD*)(ViewportAddress + 0x2EB) == 200) //Soccer Ball
	{
		goto EXIT;
	}

	lpHealthBar = gHealthBar.GetHealthBar(*(WORD*)(ViewportAddress + 0x1DC), *(BYTE*)(ViewportAddress + 0x84));

	if (lpHealthBar == 0)
	{
		goto EXIT;
	}

	LifeBarWidth = 200.0f;

	PosX = ImgCenterScreenPosX(LifeBarWidth);

	PosY = 10.0f;

	EnableAlphaTest(true);

	glColor3f(0.0f, 0.0f, 0.0f);

	RenderColor(PosX, (float)PosY, LifeBarWidth, 12.0f);

	glColor3f(0.50f, 0.00f, 0.00f);

	RenderColor(PosX + 2.0f, (float)PosY + 2, ((LifeBarWidth - 4) * lpHealthBar->rateHP) / 100, 8.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	EnableAlphaTest(false);

	MonsterName = (char*)(ViewportAddress + 0x1C1);

	EnableAlphaTest(true);

	wsprintf(LifeDisplay, "%s: %d%%", MonsterName, lpHealthBar->rateHP);

	SetBackgroundTextColor = Color4f(0, 0, 0, 0);

	SetTextColor = Color4f(255, 255, 255, 255);

	RenderText(240, (int)PosY + 2, LifeDisplay, (160 * WindowWidth / 640), 7, 1);

	DisableAlphaBlend();

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}