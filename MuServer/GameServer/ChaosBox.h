#pragma once

#include "ItemManager.h"
#include "ProtocolDefines.h"
#include "User.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAOS_MIX_SEND
{
	PBMSG_HEAD header; // C1:86
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CChaosBox
{
public:

	CChaosBox();

	~CChaosBox();

	void ChaosBoxInit(LPOBJ lpObj);

	void ChaosBoxItemDown(LPOBJ lpObj, int slot);

	void ChaosBoxItemKeep(LPOBJ lpObj, int slot);

	bool Wing2Mix(LPOBJ lpObj);

	bool FruitMix(LPOBJ lpObj);

	bool DinorantMix(LPOBJ lpObj);

	bool DevilSquareMix(LPOBJ lpObj);

	bool BloodCastleMix(LPOBJ lpObj);

	bool PlusItemLevelMix(LPOBJ lpObj, int type);

	bool Wing1Mix(LPOBJ lpObj);

	bool ChaosItemMix(LPOBJ lpObj);

	void CGChaosMixRecv(int aIndex);

	void CGChaosMixCloseRecv(int aIndex);

	void GCChaosBoxSend(LPOBJ lpObj);

	void GCChaosMixSend(int aIndex, BYTE result, CItem* lpItem);
};

extern CChaosBox gChaosBox;