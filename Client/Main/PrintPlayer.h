#pragma once

#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))

#define MAX_CHARACTER_LEVEL 400

void InitPrintPlayer();

void PrintDrawCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

void PrintDrawCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

void PrintPlayerViewHP1(char* a, char* b);

void PrintPlayerViewHP2(float a, float b, DWORD c, float d, float e);

void PrintPlayerViewMP1(char* a, char* b);

void PrintPlayerViewMP2(float a, float b, DWORD c, float d, float e);

void PrintPlayerViewBP1(char* a, char* b);

void PrintPlayerViewBP2(float a, float b, DWORD c, float d, float e);

void PrintPlayerViewExperience(char* a, char* b);

void PrintPlayerViewLevelUpPoint(char* a, char* b);

void PrintPlayerViewStrength(char* a, char* b);

void PrintPlayerViewDexterity(char* a, char* b);

void PrintPlayerViewVitality(char* a, char* b);

void PrintPlayerViewEnergy(char* a, char* b);

void PrintPlayerSetAttackSpeed();

void PrintDamageOnScreenHP(DWORD a, DWORD b, DWORD c, DWORD d, DWORD e);

void gObjSetExperienceTable();

void PrintExperienceBar(float a, float b, float c, float d);

void PrintExperienceNumber(float a, float b, DWORD c, float d, float e);

extern DWORD ViewIndex;

extern DWORD ViewReset;

extern DWORD ViewGrandReset;

extern DWORD ViewValue;

extern DWORD ViewPoint;

extern DWORD ViewCurHP;

extern DWORD ViewMaxHP;

extern DWORD ViewCurMP;

extern DWORD ViewMaxMP;

extern DWORD ViewCurBP;

extern DWORD ViewMaxBP;

extern DWORD ViewDamageHP;

extern DWORD ViewExperience;

extern DWORD ViewNextExperience;

extern DWORD ViewStrength;

extern DWORD ViewDexterity;

extern DWORD ViewVitality;

extern DWORD ViewEnergy;

extern DWORD ViewAddStrength;

extern DWORD ViewAddDexterity;

extern DWORD ViewAddVitality;

extern DWORD ViewAddEnergy;

extern DWORD ViewPhysiSpeed;

extern DWORD ViewMagicSpeed;