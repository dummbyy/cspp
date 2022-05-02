#include "includes.h"
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false;

bool show_main = true;

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);


// Cheat options
bool esp  = false;


// Glow Variables
bool glow = false;
ImColor teamColor;
ImColor enemyColor;

DWORD clientModule;
DWORD engineModule;
DWORD localPlayer;



void InitializeHack()
{
	clientModule = (uintptr_t)GetModuleHandle("client.dll");
	engineModule = (uintptr_t)GetModuleHandle("client.dll");

	clientModule = *(uintptr_t*)(clientModule + dwLocalPlayer);
	iShotsFired = (int*)(localPlayer + m_iShotsFired);
	viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
	aimRecoilPunch = (Vector3*)(localPlayer + m_aimPunchAngle);
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitializeHack();
		InitImGui(pDevice);
		init = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		show_main = !show_main;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_main)
	{
		ImGui::Begin("CSGO++ Ultimate Hack");
		// ESP
		ImGui::Text("Esp Settings");
		ImGui::Checkbox("ESP", &esp);

		// Bhop
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Checkbox("Bhop", &bhop);

		// No Recoil
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Checkbox("No Recoil", &nrc);
		if (nrc)
		{
			ImGui::SliderFloat("No Recoil Amount", &nrc_amount, 0, 1);
		}
		// Trigger Bot
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Triggerbot Settings");
		ImGui::Checkbox("Enabled", &triggerbot);
		if (triggerbot)
		{
			ImGui::Spacing();
			ImGui::Checkbox("Enable Custom Delay", &triggerBotDelay); 
			if (triggerBotDelay)
			{
				ImGui::SliderInt("Custom Delay (ms)", &triggerCustomDelay, 0, 250);
			} else {
				ImGui::Checkbox("Random Delay", &randomTrigger);
			}

		}

		ImGui::End();
	}


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, BhopThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, NRCThread,  hMod, 0, nullptr);
		CreateThread(nullptr, 0, TriggerThread, hMod, 0, nullptr);

		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
