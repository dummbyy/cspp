#pragma once
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

extern bool bhop = false;


extern DWORD WINAPI BhopThread(LPVOID lp)
{
	DWORD gameModule = (DWORD)GetModuleHandle("client.dll");
	DWORD localPlayer = *(DWORD*)(gameModule + dwLocalPlayer);
	while (true)
	{
		if (bhop)
		{
			DWORD flag = *(BYTE*)(localPlayer + m_fFlags);
			if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0))
			{
				*(DWORD*)(gameModule + dwForceJump) = 6;
			}
		}
	}
}