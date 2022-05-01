#pragma once

struct MAIN_FILE_INFO
{
	BYTE LauncherType;
	char LauncherName[32];
	char CustomerName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	char ClientName[32];
	char PluginName[32];
	DWORD ClientCRC32;
	DWORD PluginCRC32;
	DWORD DWMaxAttackSpeed;
	DWORD DKMaxAttackSpeed;
	DWORD FEMaxAttackSpeed;
	DWORD MGMaxAttackSpeed;
	DWORD ReconnectTime;
};

class CProtect
{
public:

	CProtect();

	~CProtect();

	bool ReadMainFile(char* name);

	void CheckLauncher();

	void CheckInstance();

	void CheckClientFile();

	void CheckPluginFile();

public:

	MAIN_FILE_INFO m_MainInfo;

	DWORD m_ClientFileCRC;
};

extern CProtect gProtect;