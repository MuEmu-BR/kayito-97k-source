#include "stdafx.h"
#include "Controller.h"
#include "GuildLogo.h"
#include "HackCheck.h"
#include "Interface.h"
#include "Language.h"
#include "PacketManager.h"
#include "Patchs.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "SkyDome.h"
#include "TrayMode.h"
#include "Util.h"
#include "WeaponView.h"
#include "Window.h"

HINSTANCE hins;

extern "C" _declspec(dllexport) void _cdecl EntryProc()
{
	if (gProtect.ReadMainFile("Data\\Local\\ClientInfo.bmd") == false)
	{
		MessageBoxA(NULL, "Read file incorrect or not exists", "Error", MB_OK);

		ExitProcess(0);
	}

	gProtect.CheckLauncher();

	gProtect.CheckInstance();

	gProtect.CheckClientFile();

	//InitConsole();

	InitPatchs();

	gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

	gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

	InitHackCheck();

	InitProtocol();

	gLanguage.Load();

	InitPrintPlayer();

	InitReconnect();

	gResolution.Init();

	gInterface.Init();

	gSkyDome.Load();

	gWeaponView.Init();

	gGuildLogo.Init();

	gProtect.CheckPluginFile();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			hins = (HINSTANCE)hModule;

			gWindow.WindowModeLoad(hins);

			gController.Load(hins);

			gTrayMode.Init(hins);

			break;
		}

		case DLL_PROCESS_DETACH:
		{
			break;
		}

		case DLL_THREAD_ATTACH:
		{
			break;
		}

		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return 1;
}