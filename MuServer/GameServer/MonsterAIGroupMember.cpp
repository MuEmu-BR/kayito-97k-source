#include "stdafx.h"
#include "MonsterAIGroupMember.h"

CMonsterAIGroupMember::CMonsterAIGroupMember()
{
	this->Reset();
}

CMonsterAIGroupMember::~CMonsterAIGroupMember()
{

}

void CMonsterAIGroupMember::Reset()
{
	this->m_GroupNumber = -1;

	this->m_Guid = -1;

	this->m_Class = -1;

	this->m_Rank = -1;

	this->m_StartAI = 0;

	this->m_AI01 = 0;

	this->m_AI02 = 0;

	this->m_AI03 = 0;

	this->m_CreateType = -1;

	this->m_MapNumber = -1;

	this->m_StartX = -1;

	this->m_StartY = -1;

	this->m_StartDir = -1;

	this->m_RegenType = -1;

	this->m_ObjIndex = -1;
}