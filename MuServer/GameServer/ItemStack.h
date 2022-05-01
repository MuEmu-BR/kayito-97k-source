#pragma once

struct ITEM_STACK_INFO
{
	int Index;
	int MaxStack;
	int CreateItemIndex;
};

class CItemStack
{
public:

	CItemStack();

	~CItemStack();

	void Load(char* path);

	int GetItemMaxStack(int index);

	int GetCreateItemIndex(int index);

private:

	std::map<int, ITEM_STACK_INFO> m_ItemStackInfo;
};

extern CItemStack gItemStack;