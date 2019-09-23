#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <optional>
#include <complex>
#include <limits>
#include <cstddef>
#include <utility>
#include <optional>

#include <D3D11.h>
#include <DirectXMath.h>

#include "sol.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "dx11/imgui_impl_dx11.h"
#include "dx11/imgui_impl_win32.h"

#include "ImRenderer.hpp"
#include "math.hpp"
#include "structs.hpp"
#include "memory.hpp"
#include "settings.hpp"
#include "utils.hpp"
#include "offsets.hpp"
#include "menu.hpp"
#include "wndproc.hpp"
#include "hooks.hpp"
#include "lua_api.hpp"

#include "syscall.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "shlwapi.lib")

#if _DEBUG
#pragma comment(lib, "Debug\\libMinHook.x64.lib")
#else
#pragma comment(lib, "Release\\libMinHook.x64.lib")
#endif

namespace G
{
	extern bool			HooksInitialized;
	extern bool			PressedKeys[];
	extern bool			MenuOpened;
	extern HWND			MainWindow;
	extern bool			PresentExecute;
	extern HMODULE		MainhModule;


	extern CPointer<CBaseEntity> pGameLocalPlayer;
	extern CBaseEntity *LocalPlayer;

	extern std::string	LuaPath;
	extern sol::state	Lua;
}

typedef struct {
	bool Opened;

	struct
	{
		bool Enabled;
		bool DrawBox;
		bool DrawDistance;
		bool WeaponTrace;

		bool ShowEnemies;
		bool ShowFriendly;
		bool Showbots;
		bool Thirdperson;
		bool FullName;
		bool ShortName;
		float ZoomMulti;
		float ScopeShadow;
	} Visuals;

	struct
	{
		bool Enabled;
		bool AimFriendly;
		int Key;
		float Fov;
	} Aimbot;

	struct
	{
		bool Enabled;
		int Key;
		struct
		{
			bool Green;
			bool Yellow;
			bool Red;
		} Filter;
	} Triggerbot;

	struct
	{
		float Timescale;
		bool DrawPenetrationIndicator;
		bool DrawDistanceInScope;
		bool DrawPlaneIndicator;
		bool ArcadeAutoTargeting;
		bool UnitGlow;
		bool DrawBombIndicator;
		bool HideBoxWhileGlow;
	} Misc;

	struct
	{
		bool Visuals;
		bool Misc;
		bool Triggerbot;
		bool Aimbot;
	} Menu;
} G_Vars_t;

extern G_Vars_t Vars;