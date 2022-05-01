#pragma once

#include "Item.h"
#include "User.h"

enum eItemOption
{
	ITEM_OPTION_ADD_PHYSI_DAMAGE = 80,
	ITEM_OPTION_ADD_MAGIC_DAMAGE = 81,
	ITEM_OPTION_ADD_DEFENSE_SUCCESS_RATE = 82,
	ITEM_OPTION_ADD_DEFENSE = 83,
	ITEM_OPTION_ADD_CRITICAL_DAMAGE_RATE = 84,
	ITEM_OPTION_ADD_HP_RECOVERY_RATE = 85,
	ITEM_OPTION_ADD_MONEY_AMOUNT_DROP_RATE = 86,
	ITEM_OPTION_MUL_DEFENSE_SUCCESS_RATE = 87,
	ITEM_OPTION_ADD_DAMAGE_REFLECT = 88,
	ITEM_OPTION_ADD_DAMAGE_REDUCTION = 89,
	ITEM_OPTION_MUL_MP = 90,
	ITEM_OPTION_MUL_HP = 91,
	ITEM_OPTION_ADD_EXCELLENT_DAMAGE_RATE = 92,
	ITEM_OPTION_ADD_PHYSI_DAMAGE_BY_LEVEL = 93,
	ITEM_OPTION_MUL_PHYSI_DAMAGE = 94,
	ITEM_OPTION_ADD_MAGIC_DAMAGE_BY_LEVEL = 95,
	ITEM_OPTION_MUL_MAGIC_DAMAGE = 96,
	ITEM_OPTION_ADD_SPEED = 97,
	ITEM_OPTION_ADD_HUNT_HP = 98,
	ITEM_OPTION_ADD_HUNT_MP = 99,
	ITEM_OPTION_ADD_WING_HP = 100,
	ITEM_OPTION_ADD_WING_MP = 101,
	ITEM_OPTION_ADD_IGNORE_DEFENSE_RATE = 102,
	ITEM_OPTION_ADD_BP = 103,
	ITEM_OPTION_MUL_BP = 104,
};

struct ITEM_OPTION_INFO
{
	int Index;
	int OptionIndex;
	int OptionValue;
	int ItemMinIndex;
	int ItemMaxIndex;
	int ItemSkillOption;
	int ItemLuckOption;
	int ItemAddOption;
	int ItemExceOption;
};

class CItemOption
{
public:

	CItemOption();

	~CItemOption();

	void Load(char* path);

	bool GetItemOption(int index, CItem* lpItem);

	void CalcItemCommonOption(LPOBJ lpObj, bool flag);

	void InsertOption(LPOBJ lpObj, CItem* lpItem, bool flag);

private:

	std::map<int, std::vector<ITEM_OPTION_INFO>> m_ItemOptionInfo;
};

extern CItemOption gItemOption;