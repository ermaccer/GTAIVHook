#include "includes.h"
#include <iostream>
#include "code/gtaivmenu.h"
#include "code/gtaivutils.h"
#include "pattern/Hooking.Patterns.h"
#include "MemoryMgr.h"
#include <d3d9.h>

using namespace Memory::VP;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


EndScene oEndScene = NULL;
WNDPROC oWndProc;


DWORD				 pDevicePointer;
HWND				 hWindow;
DWORD				 dwEndSceneAddress;
DWORD				 dwCreateWindowAddress;
DWORD				 dwProcessFunctionJMPAddress;
DWORD				 dwProcessFunctionAddress;
bool				 set = false;

void ImGuiInputWatcher()
{
	while (true)
	{
		if (TheMenu->bUpdatePos)
		{
			ImGuiIO& io = ImGui::GetIO();
			POINT mPos;
			GetCursorPos(&mPos);
			ScreenToClient(hWindow, &mPos);
			io.MousePos.x = mPos.x;
			io.MousePos.y = mPos.y;
			io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
			io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
			io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
		}
		//printf("pos %d %d mouse %d %d\n", mPos.x, mPos.y, io.MouseDown[0], io.MouseDown[1]);
		Sleep(1);
	}
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_F1)
			TheMenu->bIsActive ^= 1;
		break;
	case WM_KILLFOCUS:
		TheMenu->bFocused = false;
		break;
	case WM_SETFOCUS:
		TheMenu->bFocused = true;
		break;
	default:
		break;
	}

	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void InitImGui(LPDIRECT3DDEVICE9 dev)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)WndProc);
	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ImGuiInputWatcher), nullptr, 0, nullptr);
	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX9_Init(dev);
}


int EndScene_Hook(int a1)
{
	auto pDevice = *(LPDIRECT3DDEVICE9*)pDevicePointer;

	if (!set)
	{
		InitImGui(pDevice);
		set = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = false;
	if (TheMenu->GetActiveState())
	{
		TheMenu->Draw();
	}

	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());


	return (*(int(__cdecl **)(int))(**(int **)(a1 + 4524) + 168))(*(int *)(a1 + 4524));
}

void CustomProcess()
{
	TheMenu->Process();
}

void __declspec(naked) Process_Hook()
{
	CustomProcess();

	_asm {
		lea     eax, [esp + 16]
		push	eax
		jmp dwProcessFunctionJMPAddress
	}

}

HWND __stdcall CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND result = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	hWindow = result;
	return result;
}


void Init()
{

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	pDevicePointer = 0x0;

	// patterns because this code seems to be drm'ed

	pDevicePointer = *hook::get_pattern<DWORD>("83 C4 0C A1 ? ? ? ? A3", 4);

	if (!pDevicePointer)
	{
		MessageBox(0, L"Failed to obtain device address!", L"GTAIVHook", MB_ICONWARNING);
		return;
	}

	auto pattern = hook::pattern("8B 44 24 04 8B 80 AC 11 00 00 8B 08 89 44 24 04 FF A1 A8 00 00 00");
	if (!pattern.count_hint(1).empty())
	{
		auto ptr = pattern.get(0).get<int>(0);
		dwEndSceneAddress = (int)ptr;
		InjectHook(dwEndSceneAddress, EndScene_Hook, PATCH_JUMP);
	}


	pattern = hook::pattern("8D 44 24 10 50 FF D5 8D 44 24 10");
	if (!pattern.count_hint(1).empty())
	{
		auto ptr = pattern.get(0).get<int>(0);
		dwProcessFunctionAddress = (int)ptr;
		dwProcessFunctionJMPAddress = dwProcessFunctionAddress + 0x5;
		InjectHook(dwProcessFunctionAddress, Process_Hook, PATCH_JUMP);
	}


	Patch<int>(_ivaddr(0xE733C0), (int)CreateWindowExA_Hook);

	TheMenu->Initialize();
	InjectHook(_ivaddr(0x8F6B10), GTAIV::ProcessInput, PATCH_CALL);
}



extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		 Init();
	}
}


