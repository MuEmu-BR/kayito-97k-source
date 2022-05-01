#include "stdafx.h"
#include "ItemStack.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CItemStack gItemStack;

CItemStack::CItemStack()
{
	this->m_ItemStackInfo.clear();
}

CItemStack::~CItemStack()
{

}

void CItemStack::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemStackInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			ITEM_STACK_INFO info;

			info.Index = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(), lpMemScript->GetAsNumber()));

			info.MaxStack = lpMemScript->GetAsNumber();

			info.CreateItemIndex = lpMemScript->GetAsNumber();

			if (info.CreateItemIndex != -1)
			{
				info.CreateItemIndex = SafeGetItem(GET_ITEM(info.CreateItemIndex, lpMemScript->GetAsNumber()));
			}

			this->m_ItemStackInfo.insert(std::pair<int, ITEM_STACK_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CItemStack::GetItemMaxStack(int index)
{
	std::map<int, ITEM_STACK_INFO>::iterator it = this->m_ItemStackInfo.find(index);

	if (it == this->m_ItemStackInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.MaxStack;
	}
}

int CItemStack::GetCreateItemIndex(int index)
{
	std::map<int, ITEM_STACK_INFO>::iterator it = this->m_ItemStackInfo.find(index);

	if (it == this->m_ItemStackInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.CreateItemIndex;
	}
}