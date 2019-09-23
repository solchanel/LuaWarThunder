#include "main.hpp"
#include <Shlwapi.h>

namespace G
{
	bool	HooksInitialized;
	bool	PressedKeys[256] = { 0 };
	bool	MenuOpened;
	HWND	MainWindow;
	bool	PresentExecute;
	HMODULE	MainhModule;

	CPointer<CBaseEntity> pGameLocalPlayer;
	CBaseEntity *LocalPlayer;

	std::string LuaPath;
	sol::state	Lua;

	CRITICAL_SECTION Luacs;
}

G_Vars_t Vars;

void InputThread()
{
	std::string in;
	while (std::cin >> in)
	{
		EnterCriticalSection(&G::Luacs);

		if (in == "restart") {
			std::cout << "----------------- Restarting Lua state -----------------" << std::endl;
			G::Lua.safe_script_file(G::LuaPath);
		}

		LeaveCriticalSection(&G::Luacs);
		in.clear();
	}
}

void Main(HINSTANCE hInstance)
{
	HMODULE hAces = GetModuleHandle(NULL);

	if (!hAces)
		return;

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	freopen("CON", "r", stdin);

	Memory::Init();

	while (!(G::MainWindow = FindWindowA(NULL, "War Thunder")))
		Sleep(500);

	while (!GetModuleHandle("dxgi") || !GetModuleHandle("D3D11.dll"))
		Sleep(500);

	auto crash_1 = (CBaseEntity *)U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "4C 39 15 ?? ?? ?? ?? 45 8B DA"), OFFSET_OP::CMP);
	auto crash_2 = (GameInfo **)U::GetOffsetX64<DWORD64 *>(U::FindPattern<DWORD64>(hAces, "48 8B 15 ?? ?? ?? ?? 48 8B 82 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00"), OFFSET_OP::MOV);
	while (!crash_1 || !*crash_2)
		Sleep(200);

	Settings = new CSettings();
	Settings->Load();

	char Path[MAX_PATH];
	GetModuleFileName((HMODULE)hInstance, Path, sizeof(Path));
	PathRemoveFileSpec(Path);

	std::sprintf(Path, "%s\\run.lua", Path);
	G::LuaPath = Path;

	InitializeCriticalSection(&G::Luacs);
	EnterCriticalSection(&G::Luacs);

	auto LuaRenderer = G::Lua["Renderer"].get_or_create<sol::table>();

	LuaRenderer.set_function("DrawString", &LuaRenderer::DrawString);
	LuaRenderer.set_function("DrawLine", &LuaRenderer::DrawLine);
	LuaRenderer.set_function("DrawCircle", &LuaRenderer::DrawCircle);
	LuaRenderer.set_function("DrawCircleFilled", &LuaRenderer::DrawCircleFilled);

	G::Lua.open_libraries();
	G::Lua.safe_script_file(G::LuaPath);

	LeaveCriticalSection(&G::Luacs);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InputThread, NULL, NULL, NULL);

	LuaAPI::LuaCallback<void()>("WarThunder_Init");


	O::Initialize();
	H::Setup();
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD Reason, LPVOID lpvReserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, (LPVOID)hInstance, NULL, NULL);
	}
	return true;
}