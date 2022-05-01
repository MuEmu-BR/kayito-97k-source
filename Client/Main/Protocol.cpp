#include "stdafx.h"
#include "Protocol.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "HWID.h"
#include "Language.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Reconnect.h"
#include "Util.h"

void InitProtocol()
{
	SetCompleteHook(0xE9, 0x00439505, &HookProtocol);
}

_declspec(naked) void HookProtocol()
{
	static DWORD jmpBack = 0x0043950B;

	_asm
	{
		Pushad;
		Push Ebp;
		Push Esi;
		Call ProtocolCoreEx;
		Popad;
		Mov Edx, Dword Ptr Ds : [0x7E11DCC] ;
		Jmp jmpBack;
	}
}

void __stdcall ProtocolCoreEx(BYTE head, BYTE* lpMsg)
{
	//LogAdd("RECV 0: %02X, 1: %02X, 2: %02X, 3: %02X, 4: %02X, 5: %02X", lpMsg[0], lpMsg[1], lpMsg[2], lpMsg[3], lpMsg[4], lpMsg[5]);

	switch (head)
	{
		case 0x15:
		{
			GCDamageRecv((PMSG_DAMAGE_RECV*)lpMsg);

			break;
		}

		case 0x17:
		{
			GCUserDieRecv((PMSG_USER_DIE_RECV*)lpMsg);

			break;
		}

		case 0x26:
		{
			GCLifeRecv((PMSG_LIFE_RECV*)lpMsg);

			break;
		}

		case 0x27:
		{
			GCManaRecv((PMSG_MANA_RECV*)lpMsg);

			break;
		}

		case 0x2C:
		{
			GCFruitResultRecv((PMSG_FRUIT_RESULT_RECV*)lpMsg);

			break;
		}

		case 0x9C:
		{
			GCRewardExperienceRecv((PMSG_REWARD_EXPERIENCE_RECV*)lpMsg);

			break;
		}

		case 0xA3:
		{
			GCQuestRewardRecv((PMSG_QUEST_REWARD_RECV*)lpMsg);

			break;
		}

		case 0xF1:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					GCConnectClientRecv((PMSG_CONNECT_CLIENT_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					GCConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					GCCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0xF3:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					GCCharacterListRecv((PMSG_CHARACTER_LIST_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					GCCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0x04:
				{
					GCCharacterRegenRecv((PMSG_CHARACTER_REGEN_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					GCLevelUpRecv((PMSG_LEVEL_UP_RECV*)lpMsg);

					break;
				}

				case 0x06:
				{
					GCLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg);

					break;
				}

				case 0x07:
				{
					GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)lpMsg);

					break;
				}

				case 0xE0:
				{
					GCNewCharacterInfoRecv((PMSG_NEW_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0xE1:
				{
					GCNewCharacterCalcRecv((PMSG_NEW_CHARACTER_CALC_RECV*)lpMsg);

					break;
				}

				case 0xE2:
				{
					GCHealthBarRecv((PMSG_HEALTH_BAR_RECV*)lpMsg);

					break;
				}
			}

			break;
		}
	}
}

void GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = lpMsg->ViewCurHP;
	}

	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = 0;
	}
}

void GCLifeRecv(PMSG_LIFE_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		ViewMaxHP = lpMsg->ViewHP;
	}

	if (lpMsg->type == 0xFF)
	{
		ViewCurHP = ((ViewCurHP == 0) ? ViewCurHP : lpMsg->ViewHP);
	}
}

void GCManaRecv(PMSG_MANA_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		ViewMaxMP = lpMsg->ViewMP;

		ViewMaxBP = lpMsg->ViewBP;
	}

	if (lpMsg->type == 0xFF)
	{
		ViewCurMP = lpMsg->ViewMP;

		ViewCurBP = lpMsg->ViewBP;
	}
}

void GCFruitResultRecv(PMSG_FRUIT_RESULT_RECV* lpMsg)
{
	if (lpMsg->result == 0 || lpMsg->result == 3 || lpMsg->result == 6 || lpMsg->result == 17)
	{
		ViewValue = lpMsg->ViewValue;

		ViewPoint = lpMsg->ViewPoint;

		ViewStrength = lpMsg->ViewStrength;

		ViewDexterity = lpMsg->ViewDexterity;

		ViewVitality = lpMsg->ViewVitality;

		ViewEnergy = lpMsg->ViewEnergy;
	}
}

void GCRewardExperienceRecv(PMSG_REWARD_EXPERIENCE_RECV* lpMsg)
{
	ViewDamageHP = lpMsg->ViewDamageHP;

	ViewExperience = lpMsg->ViewExperience;

	ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCQuestRewardRecv(PMSG_QUEST_REWARD_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewPoint = lpMsg->ViewPoint;
	}
}

void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg)
{
	ViewIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	gLanguage.SendLanguage();

	gHwid.SendHwid();
}

void GCConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg)
{
	ReconnectOnConnectAccount(lpMsg->result);
}

void GCCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg)
{
	ReconnectOnCloseClient(lpMsg->result);
}

void GCCharacterListRecv(PMSG_CHARACTER_LIST_RECV* lpMsg)
{
	ReconnectOnCharacterList();
}

void GCCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg)
{
	ReconnectOnCharacterInfo();

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	ViewReset = lpMsg->ViewReset;

	ViewGrandReset = lpMsg->ViewGrandReset;

	ViewPoint = lpMsg->ViewPoint;

	ViewCurHP = lpMsg->ViewCurHP;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewCurBP = lpMsg->ViewCurBP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewStrength = lpMsg->ViewStrength;

	ViewDexterity = lpMsg->ViewDexterity;

	ViewVitality = lpMsg->ViewVitality;

	ViewEnergy = lpMsg->ViewEnergy;

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	*(WORD*)(MAIN_VIEWPORT_STRUCT + 0x1DC) = 0;

	*(BYTE*)(MAIN_VIEWPORT_STRUCT + 0x00) = 0;

	STRUCT_DECRYPT;

	switch (*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B) & 7)
	{
		case 0:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.DWMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}

		case 1:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.DKMaxAttackSpeed >= 0xFFFF) ? 0x0F : 0x0F));

			break;
		}

		case 2:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.FEMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}

		case 3:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.MGMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}
	}

	STRUCT_ENCRYPT;
}

void GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewCurBP = lpMsg->ViewCurBP;
}

void GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg)
{
	ViewPoint = lpMsg->ViewPoint;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewExperience = lpMsg->ViewExperience;

	ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg)
{
	if (lpMsg->result >= 16 && lpMsg->result < 20)
	{
		ViewPoint = lpMsg->ViewPoint;

		ViewMaxHP = lpMsg->ViewMaxHP;

		ViewMaxMP = lpMsg->ViewMaxMP;

		ViewMaxBP = lpMsg->ViewMaxBP;

		ViewStrength = lpMsg->ViewStrength;

		ViewDexterity = lpMsg->ViewDexterity;

		ViewVitality = lpMsg->ViewVitality;

		ViewEnergy = lpMsg->ViewEnergy;
	}
}

void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;

	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCNewCharacterInfoRecv(PMSG_NEW_CHARACTER_INFO_RECV* lpMsg)
{
	ViewPoint = lpMsg->LevelUpPoint;

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	ViewStrength = lpMsg->Strength;

	ViewDexterity = lpMsg->Dexterity;

	ViewVitality = lpMsg->Vitality;

	ViewEnergy = lpMsg->Energy;

	ViewCurHP = lpMsg->Life;

	ViewMaxHP = lpMsg->MaxLife;

	ViewCurMP = lpMsg->Mana;

	ViewMaxMP = lpMsg->MaxMana;

	ViewCurBP = lpMsg->BP;

	ViewMaxBP = lpMsg->MaxBP;

	ViewReset = lpMsg->ViewReset;

	ViewGrandReset = lpMsg->ViewGrandReset;
}

void GCNewCharacterCalcRecv(PMSG_NEW_CHARACTER_CALC_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewCurBP = lpMsg->ViewCurBP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewAddStrength = lpMsg->ViewAddStrength;

	ViewAddDexterity = lpMsg->ViewAddDexterity;

	ViewAddVitality = lpMsg->ViewAddVitality;

	ViewAddEnergy = lpMsg->ViewAddEnergy;

	ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;

	ViewMagicSpeed = lpMsg->ViewMagicSpeed;
}

void GCHealthBarRecv(PMSG_HEALTH_BAR_RECV* lpMsg)
{
	gHealthBar.ClearHealthBar();

	for (int n = 0; n < lpMsg->count; n++)
	{
		MONSTER_HEALTH_BAR* lpInfo = (MONSTER_HEALTH_BAR*)(((BYTE*)lpMsg) + sizeof(PMSG_HEALTH_BAR_RECV) + (sizeof(MONSTER_HEALTH_BAR) * n));

		gHealthBar.InsertHealthBar(lpInfo->index, lpInfo->type, lpInfo->rateHP);
	}
}

void DataSend(BYTE* lpMsg, DWORD size)
{
	BYTE EncBuff[2048];

	if (gPacketManager.AddData(lpMsg, size) != false && gPacketManager.ExtractPacket(EncBuff) != false)
	{
		BYTE send[2048];

		memcpy(send, EncBuff, size);

		if (EncBuff[0] == 0xC3 || EncBuff[0] == 0xC4)
		{
			if (EncBuff[0] == 0xC3)
			{
				BYTE save = EncBuff[1];

				PACKET_DECRYPT;

				EncBuff[1] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[2], &EncBuff[1], (size - 1)) + 2;

				EncBuff[1] = save;

				send[0] = 0xC3;

				send[1] = LOBYTE(size);
			}
			else
			{
				BYTE save = EncBuff[2];

				PACKET_DECRYPT;

				EncBuff[2] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[3], &EncBuff[2], (size - 2)) + 3;

				EncBuff[2] = save;

				send[0] = 0xC4;

				send[1] = HIBYTE(size);

				send[2] = LOBYTE(size);
			}
		}

		MySend(pSocket, send, size, 0);
	}
}