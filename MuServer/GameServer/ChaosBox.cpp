#include "stdafx.h"
#include "ChaosBox.h"
#include "DSProtocol.h"
#include "ItemOptionRate.h"
#include "Log.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Shop.h"
#include "Util.h"

CChaosBox gChaosBox;

CChaosBox::CChaosBox()
{

}

CChaosBox::~CChaosBox()
{

}

void CChaosBox::ChaosBoxInit(LPOBJ lpObj)
{
	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		lpObj->ChaosBox[n].Clear();

		lpObj->ChaosBoxMap[n] = 0xFF;
	}
}

void CChaosBox::ChaosBoxItemDown(LPOBJ lpObj, int slot)
{
	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (n != slot)
		{
			lpObj->ChaosBox[n].Clear();

			lpObj->ChaosBoxMap[n] = 0xFF;

			continue;
		}

		CItem* lpItem = &lpObj->ChaosBox[n];

		lpItem->m_Level = 0;

		float dur = (float)gItemManager.GetItemDurability(lpItem->m_Index, lpItem->m_Level, lpItem->IsExcItem());

		lpItem->m_Durability = dur * (lpItem->m_Durability / lpItem->m_BaseDurability);

		lpItem->Convert(lpItem->m_Index, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption);
	}
}

void CChaosBox::ChaosBoxItemKeep(LPOBJ lpObj, int slot)
{
	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (n != slot)
		{
			lpObj->ChaosBox[n].Clear();

			lpObj->ChaosBoxMap[n] = 0xFF;

			continue;
		}
	}
}

bool CChaosBox::Wing2Mix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int FeatherCount = 0;

	int WingItemCount = 0;

	int WingItemMoney = 0;

	int ItemCount = 0;

	int ItemMoney = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(13, 14) && lpObj->ChaosBox[n].m_Level == 0)
		{
			FeatherCount++;
		}
		else if ((lpObj->ChaosBox[n].m_Index >= GET_ITEM(12, 0) && lpObj->ChaosBox[n].m_Index <= GET_ITEM(12, 2)))
		{
			WingItemCount++;

			WingItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if (lpObj->ChaosBox[n].IsExcItem() != false && lpObj->ChaosBox[n].m_Level >= 4)
		{
			ItemCount++;

			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if (ChaosCount != 1 || FeatherCount != 1 || WingItemCount != 1)
	{
		return false;
	}

	if (gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (WingItemMoney / 4000000) + (ItemMoney / 40000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing2MixRate[lpObj->AccountLevel];
	}

	if (gServerInfo.m_Wing2MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 90) ? 90 : lpObj->ChaosSuccessRate);
	}
	else
	{
		lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);
	}

	lpObj->ChaosMoney = 5000000;

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = 0;

		BYTE ItemLuckOption = 0;

		BYTE ItemAddOption = 0;

		BYTE ItemExceOption = 0;

		CRandomManager RandomManager;

		RandomManager.AddElement(GET_ITEM(12, 3), 1);

		RandomManager.AddElement(GET_ITEM(12, 4), 1);

		RandomManager.AddElement(GET_ITEM(12, 5), 1);

		RandomManager.AddElement(GET_ITEM(12, 6), 1);

		RandomManager.GetRandomElement(&ItemIndex);

		gItemOptionRate.GetItemLuckOption(4, &ItemLuckOption);

		gItemOptionRate.GetItemAddOption(4, &ItemAddOption);

		gItemOptionRate.GetItemExceOption(4, &ItemExceOption);

		gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, ItemIndex, 0, 0, 0, ItemLuckOption, ItemAddOption, -1, (ItemExceOption + (32 * (GetLargeRand() % 2))));

		gLog.Output(LOG_CHAOS_MIX, "[Wing2Mix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[Wing2Mix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::FruitMix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int CreationCount = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 22))
		{
			CreationCount++;
		}
	}

	if (ChaosCount != 1 || CreationCount != 1)
	{
		return false;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_FruitMixRate[lpObj->AccountLevel];

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 3000000;

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		BYTE ItemLevel = GetLargeRand() % 5;

		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, GET_ITEM(13, 15), ItemLevel, 0, 0, 0, 0, -1, 0);

		gLog.Output(LOG_CHAOS_MIX, "[FruitMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[FruitMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::DinorantMix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int UniriaCount = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(13, 2) && lpObj->ChaosBox[n].m_Durability == 255)
		{
			UniriaCount++;
		}
	}

	if (ChaosCount != 1 || UniriaCount != 10)
	{
		return false;
	}

	lpObj->ChaosSuccessRate = gServerInfo.m_DinorantMixRate[lpObj->AccountLevel];

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 500000;

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = GET_ITEM(13, 3);

		BYTE ItemExceOption = 0;

		gItemOptionRate.GetItemExceOption(3, &ItemExceOption);

		gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, ItemIndex, 0, 255, 0, 0, 0, -1, ItemExceOption);

		gLog.Output(LOG_CHAOS_MIX, "[DinorantMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[DinorantMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::DevilSquareMix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int EyeCount = 0;

	int EyeLevel = 0;

	int KeyCount = 0;

	int KeyLevel = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 17))
		{
			EyeCount++;

			EyeLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 18))
		{
			KeyCount++;

			KeyLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if (ChaosCount != 1 || EyeCount != 1 || KeyCount != 1 || EyeLevel != KeyLevel)
	{
		return false;
	}

	if (EyeLevel > 7 || KeyLevel > 7)
	{
		this->GCChaosMixSend(lpObj->Index, 8, 0);

		return true;
	}

	switch (EyeLevel)
	{
		case 1:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate1[lpObj->AccountLevel];

			lpObj->ChaosMoney = 100000;

			break;
		}

		case 2:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate2[lpObj->AccountLevel];

			lpObj->ChaosMoney = 200000;

			break;
		}

		case 3:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate3[lpObj->AccountLevel];

			lpObj->ChaosMoney = 400000;

			break;
		}

		case 4:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_DevilSquareMixRate4[lpObj->AccountLevel];

			lpObj->ChaosMoney = 700000;

			break;
		}
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, GET_ITEM(14, 19), EyeLevel, 1, 0, 0, 0, -1, 0);

		gLog.Output(LOG_CHAOS_MIX, "[DevilSquareMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[DevilSquareMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::BloodCastleMix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int ScrollCount = 0;

	int ScrollLevel = 0;

	int BoneCount = 0;

	int BoneLevel = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(13, 16))
		{
			ScrollCount++;

			ScrollLevel = lpObj->ChaosBox[n].m_Level;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(13, 17))
		{
			BoneCount++;

			BoneLevel = lpObj->ChaosBox[n].m_Level;
		}
	}

	if (ChaosCount != 1 || ScrollCount != 1 || BoneCount != 1 || ScrollLevel != BoneLevel)
	{
		return false;
	}

	if (ScrollLevel > 8 || BoneLevel > 8)
	{
		this->GCChaosMixSend(lpObj->Index, 8, 0);

		return true;
	}

	switch (ScrollLevel)
	{
		case 1:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate1[lpObj->AccountLevel];

			lpObj->ChaosMoney = 50000;

			break;
		}

		case 2:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate2[lpObj->AccountLevel];

			lpObj->ChaosMoney = 80000;

			break;
		}

		case 3:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate3[lpObj->AccountLevel];

			lpObj->ChaosMoney = 150000;

			break;
		}

		case 4:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate4[lpObj->AccountLevel];

			lpObj->ChaosMoney = 250000;

			break;
		}

		case 5:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate5[lpObj->AccountLevel];

			lpObj->ChaosMoney = 400000;

			break;
		}

		case 6:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_BloodCastleMixRate6[lpObj->AccountLevel];

			lpObj->ChaosMoney = 600000;

			break;
		}
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) <= lpObj->ChaosSuccessRate)
	{
		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, GET_ITEM(13, 18), ScrollLevel, 1, 0, 0, 0, -1, 0);

		gLog.Output(LOG_CHAOS_MIX, "[BloodCastleMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[BloodCastleMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::PlusItemLevelMix(LPOBJ lpObj, int type)
{
	int ChaosCount = 0;

	int BlessCount = 0;

	int SoulCount = 0;

	int ItemCount = 0;

	int ItemSlot = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 13))
		{
			BlessCount++;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 14))
		{
			SoulCount++;
		}
		else if (lpObj->ChaosBox[n].m_Level == (9 + type))
		{
			ItemCount++;

			ItemSlot = n;
		}
	}

	if (ChaosCount != 1 || SoulCount < (type + 1) || BlessCount < (type + 1) || ItemCount != 1)
	{
		return false;
	}

	switch (type)
	{
		case 0:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate1[lpObj->AccountLevel];

			break;
		}

		case 1:
		{
			lpObj->ChaosSuccessRate = gServerInfo.m_PlusItemLevelMixRate2[lpObj->AccountLevel];

			break;
		}
	}

	if (lpObj->ChaosBox[ItemSlot].m_LuckOption != 0)
	{
		lpObj->ChaosSuccessRate += gServerInfo.m_AddLuckSuccessRate2[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = 2000000 * (type + 1);

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		CItem item = lpObj->ChaosBox[ItemSlot];

		item.m_Level++;

		item.m_Durability = (float)gItemManager.GetItemDurability(item.m_Index, item.m_Level, item.IsExcItem());

		item.m_Durability = (item.m_Durability * lpObj->ChaosBox[ItemSlot].m_Durability) / item.m_BaseDurability;

		this->ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index, item, 0);

		this->GCChaosMixSend(lpObj->Index, 1, &item);

		gObjCustomLogPlusChaosMix(lpObj, type, item.m_Index);

		gLog.Output(LOG_CHAOS_MIX, "[PlusItemLevelMix][Success][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, type, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[PlusItemLevelMix][Failure][%s][%s] - (Type: %d, ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, type, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::Wing1Mix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int ChaosItem = 0;

	int ItemCount = 0;

	int ItemMoney = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;

			lpObj->ChaosBox[n].OldValue();

			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 14))
		{
			ItemCount++;

			lpObj->ChaosBox[n].OldValue();

			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if ((lpObj->ChaosBox[n].m_Index == GET_ITEM(2, 6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(4, 6) || lpObj->ChaosBox[n].m_Index == GET_ITEM(5, 7)) && lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_AddOption >= 1)
		{
			ChaosItem++;

			ItemCount++;

			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
		else if (lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_AddOption >= 1)
		{
			ItemCount++;

			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if (ChaosCount == 0 || ChaosItem == 0)
	{
		return false;
	}

	if (gServerInfo.m_Wing1MixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney / 20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_Wing1MixRate[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate * 10000;

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = 0;

		BYTE ItemOption2 = 0;

		BYTE ItemOption3 = 0;

		CRandomManager RandomManager;

		RandomManager.AddElement(GET_ITEM(12, 0), 1);

		RandomManager.AddElement(GET_ITEM(12, 1), 1);

		RandomManager.AddElement(GET_ITEM(12, 2), 1);

		RandomManager.GetRandomElement(&ItemIndex);

		gItemOptionRate.GetItemLuckOption(5, &ItemOption2);

		gItemOptionRate.GetItemAddOption(5, &ItemOption3);

		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, ItemIndex, 0, 0, 0, ItemOption2, ItemOption3, -1, 0);

		gLog.Output(LOG_CHAOS_MIX, "[Wing1Mix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[Wing1Mix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

bool CChaosBox::ChaosItemMix(LPOBJ lpObj)
{
	int ChaosCount = 0;

	int ItemCount = 0;

	int ItemMoney = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		if (lpObj->ChaosBox[n].m_Index == GET_ITEM(12, 15))
		{
			ChaosCount++;

			lpObj->ChaosBox[n].OldValue();

			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if (lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 13) || lpObj->ChaosBox[n].m_Index == GET_ITEM(14, 14))
		{
			ItemCount++;

			lpObj->ChaosBox[n].OldValue();

			ItemMoney += lpObj->ChaosBox[n].m_OldBuyMoney;
		}
		else if (lpObj->ChaosBox[n].m_Level >= 4 && lpObj->ChaosBox[n].m_AddOption >= 1)
		{
			ItemCount++;

			ItemMoney += lpObj->ChaosBox[n].m_BuyMoney;
		}
	}

	if (ChaosCount == 0 || ItemCount == 0)
	{
		return false;
	}

	if (gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel] == -1)
	{
		lpObj->ChaosSuccessRate = (ItemMoney / 20000);
	}
	else
	{
		lpObj->ChaosSuccessRate = gServerInfo.m_ChaosItemMixRate[lpObj->AccountLevel];
	}

	lpObj->ChaosSuccessRate = ((lpObj->ChaosSuccessRate > 100) ? 100 : lpObj->ChaosSuccessRate);

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate * 10000;

	if (lpObj->Money < ((DWORD)lpObj->ChaosMoney))
	{
		this->GCChaosMixSend(lpObj->Index, 2, 0);

		return true;
	}

	lpObj->Money -= lpObj->ChaosMoney;

	GCMoneySend(lpObj->Index, lpObj->Money);

	if ((GetLargeRand() % 100) < lpObj->ChaosSuccessRate)
	{
		WORD ItemIndex = 0;

		BYTE ItemLevel = 0;

		BYTE ItemOption1 = 0;

		BYTE ItemOption2 = 0;

		BYTE ItemOption3 = 0;

		CRandomManager RandomManager;

		RandomManager.AddElement(GET_ITEM(2, 6), 1);

		RandomManager.AddElement(GET_ITEM(4, 6), 1);

		RandomManager.AddElement(GET_ITEM(5, 7), 1);

		RandomManager.GetRandomElement(&ItemIndex);

		gItemOptionRate.GetItemLevelOption(2, &ItemLevel);

		gItemOptionRate.GetItemSkillOption(2, &ItemOption1);

		gItemOptionRate.GetItemLuckOption(2, &ItemOption2);

		gItemOptionRate.GetItemAddOption(2, &ItemOption3);

		GDCreateItemSend(lpObj->Index, 0xFF, 0, 0, ItemIndex, ItemLevel, 0, ItemOption1, ItemOption2, ItemOption3, -1, 0);

		gLog.Output(LOG_CHAOS_MIX, "[ChaosItemMix][Success][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}
	else
	{
		this->ChaosBoxInit(lpObj);

		this->GCChaosBoxSend(lpObj);

		this->GCChaosMixSend(lpObj->Index, 0, 0);

		gLog.Output(LOG_CHAOS_MIX, "[ChaosItemMix][Failure][%s][%s] - (ChaosSuccessRate: %d, ChaosMoney: %d)", lpObj->Account, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
	}

	return true;
}

void CChaosBox::CGChaosMixRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == false)
	{
		return;
	}

	if (lpObj->ChaosLock != false)
	{
		return;
	}

	lpObj->ChaosLock = true;

	lpObj->ChaosMoney = 0;

	lpObj->ChaosSuccessRate = 0;

	lpObj->IsChaosMixCompleted = true;

	if (this->Wing2Mix(lpObj) == true)
	{
		return;
	}

	if (this->FruitMix(lpObj) == true)
	{
		return;
	}

	if (this->DinorantMix(lpObj) == true)
	{
		return;
	}

	if (this->DevilSquareMix(lpObj) == true)
	{
		return;
	}

	if (this->BloodCastleMix(lpObj) == true)
	{
		return;
	}

	if (this->PlusItemLevelMix(lpObj, 0) == true)
	{
		return;
	}

	if (this->PlusItemLevelMix(lpObj, 1) == true)
	{
		return;
	}

	if (this->Wing1Mix(lpObj) == true)
	{
		return;
	}

	if (this->ChaosItemMix(lpObj) == true)
	{
		return;
	}

	this->GCChaosMixSend(lpObj->Index, 0, 0);
}

void CChaosBox::CGChaosMixCloseRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == false)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_BOX)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;

	this->ChaosBoxInit(lpObj);

	gObjInventoryCommit(aIndex);

	lpObj->ChaosLock = false;

	lpObj->IsChaosMixCompleted = false;

	PBMSG_HEAD pMsg;

	pMsg.set(0x87, sizeof(pMsg));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);
}

void CChaosBox::GCChaosBoxSend(LPOBJ lpObj)
{
	if (lpObj->Interface.type != INTERFACE_CHAOS_BOX || lpObj->Interface.state == 1)
	{
		return;
	}

	BYTE send[2048];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31, 0);

	int size = sizeof(pMsg);

	pMsg.type = 3;

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->ChaosBox[n].IsItem() == false)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo, lpObj->ChaosBox[n]);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);
}

void CChaosBox::GCChaosMixSend(int aIndex, BYTE result, CItem* lpItem)
{
	PMSG_CHAOS_MIX_SEND pMsg;

	pMsg.header.set(0x86, sizeof(pMsg));

	pMsg.result = result;

	if (lpItem == 0)
	{
		memset(pMsg.ItemInfo, 0xFF, sizeof(pMsg.ItemInfo));
	}
	else
	{
		gItemManager.ItemByteConvert(pMsg.ItemInfo, (*lpItem));
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	gObj[aIndex].ChaosLock = false;
}