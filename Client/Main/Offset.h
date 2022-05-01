#pragma once

#define MAIN_CHARACTER_STRUCT 0x07CF1FF4

#define MAIN_VIEWPORT_STRUCT *(DWORD*)0x07ABF5D8

#define MAIN_PARTY_MEMBER_COUNT 0x07EAA0E0

#define pViewportAddress *(DWORD*)0x07ABF5D0

#define ErrorMessage *(BYTE*)0x083A7C24

#define SceneFlag *(int*)0x005615C0

#define FontHeight *(int*)0x07D78080

#define	World *(int*)0x0055A7AC

#define m_Resolution *(int*)0x055C9E38

#define WindowWidth *(int*)0x0056156C

#define WindowHeight *(int*)0x00561570

#define g_fScreenRate_x *(float*)0x55C9B70

#define g_fScreenRate_y *(float*)0x55C9B74

#define g_hWnd *(HWND*)0x055C9FFC

#define g_hDC *(HDC*)0x055CA004

#define g_hRC *(HGLRC*)0x055CA008

#define WndProc (WNDPROC)(0x4149D0)

#define pDrawMessage ((int(__cdecl*)(LPCSTR Text, int Mode)) 0x0047FAE0)

#define ProtocolCore ((BOOL(*)(BYTE*))0x004389A0)

#define MAIN_PACKET_SERIAL 0x05826CEB

#define MAIN_STRUCT_SERIAL 0x07CF1FFC

#define MAIN_HASH_CLASS 0x055C9BC8

#define MAIN_CONNECTION_STATUS 0x05826CF0

#define PACKET_DECRYPT ((void(__thiscall*)(void*,int))0x00422DF0)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)

#define PACKET_ENCRYPT ((void(__thiscall*)(void*,int))0x00404040)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)

#define STRUCT_DECRYPT ((void(__thiscall*)(void*,void*))0x00423040)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)

#define STRUCT_ENCRYPT ((void(__thiscall*)(void*,void*))0x0043D1D0)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)

#define pSocket	*(SOCKET*)(0x055CA168)

#define CreateGuildMark ((void(__cdecl*)(int nMarkIndex, bool blend)) 0x004F0100)

#define RenderBoolean ((void(__cdecl*)(int x, int y, DWORD c)) 0x00480E00)

#define LoadImageJPG ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529740)

#define LoadImageTGA ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529BD0)

#define RenderBitmap ((void(__cdecl*)(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)) 0x005125A0)

#define DrawBigText ((void(*)(float x, float y, DWORD value, float width, float height))0x005122F0)

#define RenderColor ((void(__cdecl*)(float x, float y, float width, float height)) 0x005124C0)

#define GetPosFromAngle ((void(__cdecl*)(VAngle*,int*,int*)) 0x005113F0)

#define EnableAlphaTest ((void(__cdecl*)(BYTE)) 0x00511680)

#define EnableAlphaBlend ((void(__cdecl*)()) 0x00511710)

#define pCursorX *(int*)0x083A427C

#define pCursorY *(int*)0x083A4278

#define MouseLButton *(bool*)0x083A42C4

#define MouseLButtonPush *(bool*)0x083A4124

#define MouseUpdateTime *(DWORD*)0x07E11D28

#define MouseUpdateTimeMax *(DWORD*)0x00559BEC

#define SetTextColor *(DWORD*)0x00559C78

#define SetBackgroundTextColor *(DWORD*)0x00559C80

#define DisableAlphaBlend ((void(__cdecl*)()) 0x00511600)

#define RenderText ((int(__cdecl*)(int PosX, int PosY, LPCTSTR Text, int arg4, int arg5, int arg6)) 0x0047F650)

#define InventoryOpened *(BYTE*)0x07EAA117

#define GuildOpened *(BYTE*)0x07EAA114

#define CharacterOpened *(BYTE*)0x07EAA116

#define PartyOpened *(BYTE*)0x07EAA115