#include "main.hpp"
#include <Shlwapi.h>

inline void LuaExceptionHandler(sol::optional<std::string> maybe_msg)
{
	std::cout << "Lua is in a panic state and will now abort() the application" << std::endl;
	if (maybe_msg) {
		const std::string& msg = maybe_msg.value();
		std::cout << "\terror message: " << msg << std::endl;
	}
}

int LuaExceptionHandlerEx(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) 
{
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception) 
	{
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else 
	{
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), description.size());
		std::cout << std::endl;
	}
	return sol::stack::push(L, description);
}

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
	sol::state	Lua(sol::c_call<decltype(&LuaExceptionHandler), &LuaExceptionHandler>);

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

	O::Initialize();

	Settings = new CSettings();
	Settings->Load();

	char Path[MAX_PATH];
	GetModuleFileName((HMODULE)hInstance, Path, sizeof(Path));
	PathRemoveFileSpec(Path);

	std::sprintf(Path, "%s\\run.lua", Path);
	G::LuaPath = Path;

	InitializeCriticalSection(&G::Luacs);
	EnterCriticalSection(&G::Luacs);

	G::Lua.open_libraries();
	G::Lua.set_exception_handler(&LuaExceptionHandlerEx);

	LuaAPI::RegisterTypes();

	try 
	{
		sol::optional<sol::error> err = G::Lua.safe_script_file(G::LuaPath, sol::script_default_on_error);
		if (err) 
			std::cout << err->what() << std::endl;
	}
	catch (const sol::error &err)
	{
		std::cout << "sol::error: " << err.what() << std::endl;
	}
	catch (...)
	{
		std::exception_ptr crterr = std::current_exception();
		std::cout << "caught (...)" << std::endl;
	}

	LeaveCriticalSection(&G::Luacs);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InputThread, NULL, NULL, NULL);

	LuaAPI::LuaCallback<void()>("WarThunder_Init");

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