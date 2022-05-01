#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

// System Include
#include <windows.h>
#include <winsock2.h>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <Shellapi.h>
#include <Rpc.h>
#include <gl\GL.h>

// General Includes
#include "Offset.h"
#include "Define.h"
#include "Console.h"
#include "stb.h"

// Libraries
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"Opengl32.lib")