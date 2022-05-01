#include "stdafx.h"
#include "MiniMap.h"
#include "Util.h"

CMiniMap gMiniMap;

CMiniMap::CMiniMap()
{
	this->MiniMapSwitch = true;

	this->CurrentMap = -1;
	this->m_MiniMapFramePos[0] = 512.0f;
	this->m_MiniMapFramePos[1] = 259.0f;
	this->m_MiniMapFrameSize[0] = 128.0f;
	this->m_MiniMapFrameSize[1] = 173.0f;
	this->m_MiniMapSize[0] = 128.0f;
	this->m_MiniMapSize[1] = 128.0f;
	this->m_ZoomBtnSize[0] = 19.0f;
	this->m_ZoomBtnSize[1] = 20.0f;
	this->m_iMiniMapScale = 1;

	this->Init();
}

CMiniMap::~CMiniMap()
{

}

void CMiniMap::Init()
{
	SetCompleteHook(0xE8, 0x004BD332, &this->DrawMiniMap);
}

void CMiniMap::Toggle()
{
	if (SceneFlag == 5)
	{
		this->MiniMapSwitch ^= 1;
	}
}

void CMiniMap::LoadImages()
{
	LoadImageTGA(MINIMAP_FRAME_TEXTURE, 0x600, GL_NEAREST, GL_CLAMP, 0, 1);
}

bool CMiniMap::CheckInterfaces()
{
	if (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened)
	{
		return false;
	}

	return true;
}

void CMiniMap::DrawMiniMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	if (gMiniMap.MiniMapSwitch && gMiniMap.CheckInterfaces())
	{
		if (gMiniMap.CurrentMap != World)
		{
			char buff[256];

			sprintf_s(buff, MINIMAP_TEXTURE_PATH, World + 1);

			if (!FileExists(buff))
			{
				goto ORIGINAL;
			}

			sprintf_s(buff, MINIMAP_TEXTURE, World + 1);

			LoadImageJPG(buff, 0x601, GL_NEAREST, GL_CLAMP, 0, 1);

			gMiniMap.CurrentMap = World;
		}

		gMiniMap.CheckZoomButton();

		gMiniMap.RenderMiniMap();

		gMiniMap.RenderObjects();

		return;
	}

ORIGINAL:

	RenderBitmap(0xE9, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void CMiniMap::RenderMiniMap()
{
	this->m_CharacterPos[0] = (((*(float*)(MAIN_VIEWPORT_STRUCT + 0x10) / 100.0f)) / this->m_iMiniMapScale);

	this->m_CharacterPos[1] = ((256.0f - (*(float*)(MAIN_VIEWPORT_STRUCT + 0x14) / 100.0f)) / this->m_iMiniMapScale);

	this->m_MiniMapScaleOffset[0] = min(max((this->m_CharacterPos[0] - (64.0f * gMiniMap.m_iMiniMapScale)), 0), 128.0f);

	this->m_MiniMapScaleOffset[1] = min(max((this->m_CharacterPos[1] - (64.0f * gMiniMap.m_iMiniMapScale)), 0), 128.0f);

	float MiniMapScaleU = this->m_MiniMapScaleOffset[0] / (256.0f);

	float MiniMapScaleV = this->m_MiniMapScaleOffset[1] / (256.0f);

	EnableAlphaTest(true);

	RenderBitmap(0x601, gMiniMap.m_MiniMapFramePos[0], gMiniMap.m_MiniMapFramePos[1] + 46.0f, gMiniMap.m_MiniMapSize[0], gMiniMap.m_MiniMapSize[1], MiniMapScaleU, MiniMapScaleV, gMiniMap.m_iMiniMapScale * 0.5f, gMiniMap.m_iMiniMapScale * 0.5f, true, true);

	RenderBitmap(0x600, gMiniMap.m_MiniMapFramePos[0], gMiniMap.m_MiniMapFramePos[1], gMiniMap.m_MiniMapFrameSize[0], gMiniMap.m_MiniMapFrameSize[1], 0.0f, 0.0f, 1.0, (gMiniMap.m_MiniMapFrameSize[1] / 256.0f), true, true);
}

void CMiniMap::CheckZoomButton() // Arreglar que deje de caminar al clickear
{
	if (pCursorX >= (this->m_MiniMapFramePos[0])
	    && pCursorX <= (this->m_MiniMapFramePos[0] + this->m_MiniMapFrameSize[0])
	    && pCursorY >= (this->m_MiniMapFramePos[1])
	    && pCursorY <= (this->m_MiniMapFramePos[1] + this->m_MiniMapFrameSize[1])
	    && MouseLButton)
	{
		MouseLButton = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		this->m_iMiniMapScale = (this->m_iMiniMapScale == 1) ? 2 : 1;
	}
}

void CMiniMap::RenderObjects()
{
	DWORD ViewportAddress;

	for (int i = 0; i < MAX_MAIN_VIEWPORT; i++)
	{
		if ((ViewportAddress = (*(DWORD*)(0x07ABF5D0) + (i * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress + 0x84) == 1)
		{
			glColor3f(0.8f, 0.0f, 0.0f);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 2)
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 4)
		{
			glColor3f(1.0f, 0.0f, 1.0f);
		}

		if (ViewportAddress == MAIN_VIEWPORT_STRUCT)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
		}

		float CharPosX = this->m_MiniMapFramePos[0] + ((*(float*)(ViewportAddress + 0x10) / 100.0f) / this->m_iMiniMapScale) - this->m_MiniMapScaleOffset[0];

		float CharPosY = this->m_MiniMapFramePos[1] + 46.0f + ((256.0f - (*(float*)(ViewportAddress + 0x14) / 100.0f)) / this->m_iMiniMapScale) - this->m_MiniMapScaleOffset[1];

		RenderColor(CharPosX, CharPosY, 3.0f, 3.0f);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}