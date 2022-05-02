#pragma once
#include "csgo.hpp"
#include "Vector3.h"

extern int* iShotsFired;
extern Vector3* viewAngles;
extern Vector3* aimRecoilPunch;
extern Vector3 oPunch{ 0, 0, 0 };
extern float nrc_amount = 0.0;
extern bool nrc = false;

extern DWORD WINAPI NRCThread(LPVOID lp){
	DWORD gameModule = (DWORD)GetModuleHandle("client.dll");
	DWORD localPlayer = *(DWORD*)(gameModule + dwLocalPlayer);

	while (true)
	{
		if (nrc && !GetAsyncKeyState(VK_CAPITAL))
		{
			Vector3 punchAngle = *aimRecoilPunch * (nrc_amount * 2);
			if (*iShotsFired > 1 && GetAsyncKeyState(VK_CAPITAL))
			{
				Vector3 newAngle = *viewAngles + oPunch - punchAngle;
				newAngle.normalize();
				*viewAngles = newAngle;
			}
			oPunch = punchAngle;
		}
	}
}