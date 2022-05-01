#pragma once

void InitPatchs();

void ReduceCPU();

void ReduceRam(LPVOID lpThreadParameter);

void FixShopNpcClose();

void DecBMD();

int OpenTerrainMapping(char* FileName);

int OpenTerrainAttribute(char* FileName);

int OpenObjectsEnc(char* FileName);