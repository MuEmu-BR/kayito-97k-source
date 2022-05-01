#pragma once

class CGuildLogo
{
public:

	CGuildLogo();

	~CGuildLogo();

	void Init();

	static void DrawGuildLogo(int x, int y, DWORD c);
};

extern CGuildLogo gGuildLogo;