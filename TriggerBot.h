#pragma once
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

extern bool triggerbot = false;
extern bool randomTrigger = false;
extern bool triggerBotDelay = false;
extern int triggerCustomDelay = 0;

extern DWORD WINAPI TriggerThread(LPVOID lp)
{
	DWORD gameModule = *(DWORD*)GetModuleHandle("client.dll");
	while (true)
	{
		if (triggerbot)
		{
			DWORD localPlayer = *(DWORD*)(gameModule + dwLocalPlayer);
			int crosshair = *(int*)(localPlayer + m_iCrosshairId);
			int localTeam = *(int*)(localPlayer + m_iTeamNum);

			if (crosshair != 0 && crosshair < 64)
			{
				uintptr_t entity = *(uintptr_t*)(gameModule + dwEntityList + (crosshair - 1) * 0x10);
				int entityTeam = *(int*)(entity + m_iTeamNum);
				int entityHealth = *(int*)(entity + m_iHealth);

				if (entityTeam != localTeam && entityHealth > 0 && entityHealth < 101)
				{
					if (randomTrigger)
					{
						Sleep((rand() * 250) + 50);
						*(int*)(gameModule + dwForceAttack) = 5;
						Sleep(20);
						*(int*)(gameModule + dwForceAttack) = 4;
					}
					else {
						*(int*)(gameModule + dwForceAttack) = 5;
						Sleep(2);
						*(int*)(gameModule + dwForceAttack) = 4;
					}
				}
			}
		}
	}
	Sleep(2);
}
