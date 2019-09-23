#include "../main.hpp"

typedef bool setLocalPlayer_t(GameInfo *game, int a2);
typedef void destroyMultiplayer_t(GameInfo *game, bool a2);

D3D11_VIEWPORT viewport;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;

namespace AcesApp
{
	setLocalPlayer_t *setLocalPlayer_o;
	bool setLocalPlayer(GameInfo *game, int a2)
	{
		auto ret = setLocalPlayer_o(game, a2);
		if(G::pGameLocalPlayer.p && *G::pGameLocalPlayer.p)
			G::LocalPlayer = *G::pGameLocalPlayer.p;

		return ret;
	}

	destroyMultiplayer_t *destroyMultiplayer_o;
	void destroyMultiplayer(GameInfo *game, bool a2)
	{
		G::LocalPlayer = nullptr;
		destroyMultiplayer_o(game, a2);
	}
}

namespace AcesUnit
{
	typedef void SetArmy_t(Unit *unit, void *a2, void *a3);
	SetArmy_t *SetArmy_o;
	void SetArmy(Unit *unit, void *a2, void *a3)
	{
		SetArmy_o(unit, a2, a3);

		if (unit)
		{
			LuaAPI::LuaCallback<void(Unit *)>("WarThunder_OnRespawn", unit);

			O::ClientView->DrawTankDistance = Vars.Misc.DrawDistanceInScope;
			O::ClientView->PenetrationCrosshair = Vars.Misc.DrawPenetrationIndicator;
			O::ClientView->ThirdPerson = Vars.Visuals.Thirdperson;
			O::ClientView->PlaneAimIndicator = Vars.Misc.DrawPlaneIndicator;
			O::ClientView->PlaneAimIndicatorToPlane = Vars.Misc.DrawPlaneIndicator;
			O::ClientView->AutoTarget = Vars.Misc.ArcadeAutoTargeting;
			O::ClientView->AutoTargetLock = Vars.Misc.ArcadeAutoTargeting;

			O::ClientView->UnitGlow = Vars.Misc.UnitGlow;

			O::ClientView->DrawBombIndicator = Vars.Misc.DrawBombIndicator;

			unit->GetUnitInfo()->ZoomMultipler = Vars.Visuals.ZoomMulti;
			unit->GetUnitInfo()->ZoomCrosshairScale = Vars.Visuals.ZoomMulti;
			unit->GetUnitInfo()->ZoomShadowMulti = Vars.Visuals.ScopeShadow;
		}
	}
}

bool H::D3D(HWND hGameWindow)
{
	// Swapchain Desc
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0/*, D3D_FEATURE_LEVEL_10_1*/ };
	D3D_FEATURE_LEVEL obtainedLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	{
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.OutputWindow = G::MainWindow;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = ((GetWindowLongPtr(G::MainWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	// Swapchain
	IDXGISwapChain* pSwapChain;
	auto res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevel, sizeof(featureLevel) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext);
	if (FAILED(res))
	{
		std::cout << "Failed to create DirectX device and swapchain" << std::endl;
		return false;
	}

	// Swapchain VTable
	DWORD64* pD3D11_SwapChainVTable = (DWORD64*)pSwapChain;
	pD3D11_SwapChainVTable = (DWORD64*)pD3D11_SwapChainVTable[0];

	DWORD64* pDeviceVTable = (DWORD64*)pDevice;
	pDeviceVTable = (DWORD64*)pDeviceVTable[0];

	DWORD64* pDeviceContextVTable = (DWORD64*)pContext;
	pDeviceContextVTable = (DWORD64*)pDeviceContextVTable[0];

	Renderer::GetInstance()->Initialize();

	// Hook
	Memory::HookFunction((DWORD64)pD3D11_SwapChainVTable[8], &D3D11Present_h, (void**)&D3D11Present_o, false);
	//Memory::HookFunction((DWORD64)pD3D11_SwapChainVTable[50], &D3D11ClearRenderTargetView_h, (void**)&D3D11ClearRenderTargetView_o, false);
	//Memory::HookFunction((DWORD64)pDeviceVTable[24], &D3D11CreateQuery_h, (void**)&D3D11CreateQuery_o, false);
	//Memory::HookFunction((DWORD64)pDeviceContextVTable[8], &D3D11PSSetShaderResources_h, (void**)&D3D11PSSetShaderResources_o, false);
	//Memory::HookFunction((DWORD64)pDeviceContextVTable[12], &D3D11DrawIndexed_h, (void**)&D3D11DrawIndexed_o, false);

	G::HooksInitialized = true;

	for (;; Sleep(100))
		Sleep(100);

	// Release
	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

	// Done
	return true;
}

void H::Setup()
{
	WndProc_o = (WNDPROC)SetWindowLongPtr(G::MainWindow, GWLP_WNDPROC, (LONG_PTR)WndProc_h);

	HMODULE Base = GetModuleHandle(NULL);

	std::cout << "Base: 0x" << std::hex << Base << std::endl;
	
	// 4C 8B DC 53 55 56 48 83
	Memory::HookFunction(U::FindPattern<DWORD64>(Base, "4C 8B DC 53 55 56 48 83"), &AcesApp::setLocalPlayer, (void **)&AcesApp::setLocalPlayer_o, true);

	// 40 53 55 56 57 41 56 48 83 EC 50 33
	Memory::HookFunction(U::FindPattern<DWORD64>(Base, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 50 33 FF C7 44 24 ? ? ? ? ? "),
		&AcesApp::destroyMultiplayer, (void **)&AcesApp::destroyMultiplayer_o, true);

	// 40 53 56 57 48 81 EC
	Memory::HookFunction(U::FindPattern<DWORD64>(Base, "40 53 56 57 48 81 EC"), &AcesUnit::SetArmy, (void **)&AcesUnit::SetArmy_o, true);

	H::D3D(G::MainWindow);
}