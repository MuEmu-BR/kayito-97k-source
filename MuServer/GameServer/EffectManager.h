#pragma once

#include "Effect.h"
#include "Protocol.h"
#include "User.h"

#define MAX_EFFECT 71
#define MAX_EFFECT_LIST 16

#define EFFECT_RANGE(x) (((x)<0)?false:((x)>=MAX_EFFECT)?false:true)

enum eEffectNumber
{
	EFFECT_GREATER_DAMAGE = 1,
	EFFECT_GREATER_DEFENSE = 2,
	EFFECT_MANA_SHIELD = 4,
	EFFECT_BP_RECOVERY = 7,
	EFFECT_GREATER_LIFE = 8,
	EFFECT_DISABLE_MAGIC = 12,
	EFFECT_POISON = 55,
	EFFECT_ICE = 56,
	EFFECT_ICE_ARROW = 57,
	EFFECT_FIRE_SLASH = 58,
	EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY = 63,
	EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY = 64,
};

enum eEffectState
{
	EFFECT_STATE_POISON = 0x1,
	EFFECT_STATE_ICE = 0x2,
	EFFECT_STATE_GREATER_DAMAGE = 0x4,
	EFFECT_STATE_GREATER_DEFENSE = 0x8,
	EFFECT_STATE_GREATER_LIFE = 0x10,
	EFFECT_STATE_ICE_ARROW = 0x20,
	EFFECT_STATE_FIRE_SLASH = 0x40,
	EFFECT_STATE_MANA_SHIELD = 0x100,
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_EFFECT_STATE_SEND
{
	PBMSG_HEAD header; // C1:07
	BYTE state;
	WORD effect;
	BYTE index[2];
};

struct PMSG_PERIODIC_EFFECT_SEND
{
	PBMSG_HEAD header; // C1:2D
	WORD group;
	WORD value;
	BYTE state;
	DWORD time;
};

struct PMSG_PARTY_EFFECT_LIST
{
#pragma pack(1)
	BYTE effect;
	DWORD count;
#pragma pack()
};

//**********************************************//
//**********************************************//
//**********************************************//

struct EFFECT_INFO
{
	int Index;
	int Group;
	int ItemIndex;
	char Name[32];
	int Save;
	int Type;
	int Flag;
	int Count;
	int Value[4];
};

class CEffectManager
{
public:

	CEffectManager();

	~CEffectManager();

	void Init();

	void Load(char* path);

	void SetInfo(EFFECT_INFO info);

	EFFECT_INFO* GetInfo(int index);

	EFFECT_INFO* GetInfoByItem(int ItemIndex);

	void MainProc();

	bool AddEffect(LPOBJ lpObj, bool type, int index, int count, WORD value1, WORD value2, WORD value3, WORD value4);

	bool DelEffect(LPOBJ lpObj, int index);

	bool DelEffectByGroup(LPOBJ lpObj, int group);

	CEffect* GetEffect(LPOBJ lpObj, int index);

	CEffect* GetEffectByGroup(LPOBJ lpObj, int group);

	bool CheckEffect(LPOBJ lpObj, int index);

	bool CheckEffectByGroup(LPOBJ lpObj, int group);

	void InsertEffect(LPOBJ lpObj, CEffect* lpEffect);

	void RemoveEffect(LPOBJ lpObj, CEffect* lpEffect);

	bool ConvertEffectByte(CEffect* lpEffect, BYTE* lpMsg);

	void EffectByteConvert(BYTE* lpMsg, CEffect* lpEffect);

	int GenerateEffectList(LPOBJ lpObj, BYTE* lpMsg, int* size);

	int GeneratePartyEffectList(LPOBJ lpObj, BYTE* lpMsg, int* size);

	bool CheckImmobilizeEffect(LPOBJ lpObj);

	void ClearAllEffect(LPOBJ lpObj);

	void ClearDebuffEffect(LPOBJ lpObj, int count);

	bool GetEffectState(BYTE effect, WORD* result);

	void PeriodicEffect(LPOBJ lpObj, CEffect* lpEffect);

	void GCEffectStateSend(LPOBJ lpObj, BYTE state, BYTE effect);

private:

	EFFECT_INFO m_EffectInfo[MAX_EFFECT];
};

extern CEffectManager gEffectManager;