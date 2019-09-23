#include "main.hpp"

const char* keyNames[] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

ID3D11RenderTargetView *mainRenderTargetView;
D3D11Present_t *D3D11Present_o = nullptr;
HRESULT WINAPI D3D11Present_h(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool setup = false;
	if (!setup)
	{
		pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		GUI_Init(G::MainWindow, pDevice, pContext);
		//pContext->RSGetViewports(&vps, &viewport);

		//ImGui_ImplDX11_CreateDeviceObjects();

		if (mainRenderTargetView)
			mainRenderTargetView->Release();

		ID3D11Texture2D* pBackBuffer = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer))))
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			pDevice->CreateRenderTargetView(pBackBuffer, /*NULL*/&desc, &mainRenderTargetView);
			pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
			pBackBuffer->Release();
		}
		else
		{
			std::cout << "error pSwapChain->GetBuffer" << std::endl;
		}

		setup = !setup;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = G::MenuOpened;

	if (G::MenuOpened)
	{
		ImGui::Begin("Gaijin CCCP Spy", &G::MenuOpened, ImVec2(230, 140), 0.9f);
		{
			ImGui::Checkbox("Visuals", &Vars.Menu.Visuals);
			ImGui::Checkbox("Aimbot", &Vars.Menu.Aimbot);
			ImGui::Checkbox("Triggerbot", &Vars.Menu.Triggerbot);
			ImGui::Checkbox("Misc", &Vars.Menu.Misc);

			ImGui::Separator();
			ImGui::Columns(2, ("##config-settings"), false);

			if (ImGui::Button(("Save Config"), ImVec2(93.f, 20.f)))
				Settings->Save();

			ImGui::NextColumn();

			if (ImGui::Button(("Load Config"), ImVec2(93.f, 20.f)))
				Settings->Load();

			ImGui::Columns(1);
		}
		ImGui::End();

		if (Vars.Menu.Aimbot)
		{
			ImGui::Begin("Aimbot Settings", &Vars.Menu.Aimbot, ImVec2(230, 180), 0.9f);
			{
				ImGui::Checkbox("Enable Aimbot", &Vars.Aimbot.Enabled);
				ImGui::Combo("Aimbot Lock Key", &Vars.Aimbot.Key, keyNames, ARRAYSIZE(keyNames));
				ImGui::SliderFloat("Aimbot FOV", &Vars.Aimbot.Fov, 1.f, 360.f, "%.1f");
				ImGui::Checkbox("Aim Friendly", &Vars.Aimbot.AimFriendly);
			}
			ImGui::End();
		}

		if (Vars.Menu.Visuals)
		{
			ImGui::Begin("Visual Settings", &Vars.Menu.Visuals, ImVec2(230, 180), 0.9f);
			{
				ImGui::Checkbox("Show bots", &Vars.Visuals.Showbots);
				ImGui::Checkbox("Show friends", &Vars.Visuals.ShowFriendly);
				ImGui::Checkbox("Show enemies", &Vars.Visuals.ShowEnemies);
				ImGui::Checkbox("Draw distance", &Vars.Visuals.DrawDistance);
				ImGui::Checkbox("Draw weapon traces", &Vars.Visuals.WeaponTrace);

				if (ImGui::Checkbox("Show Shortname", &Vars.Visuals.ShortName) && Vars.Visuals.FullName)
					Vars.Visuals.FullName = !Vars.Visuals.ShortName;

				if (ImGui::Checkbox("Show Fullname", &Vars.Visuals.FullName) && Vars.Visuals.ShortName)
					Vars.Visuals.ShortName = !Vars.Visuals.FullName;

				if (ImGui::Checkbox("Thirdperson (simulation)", &Vars.Visuals.Thirdperson))
					O::ClientView->ThirdPerson = Vars.Visuals.Thirdperson;

				if (ImGui::SliderFloat("Zoom Shadow Multiplier", &Vars.Visuals.ScopeShadow, 20.f, 200.f, "%.0f"))
				{
					G::LocalPlayer->GetUnit()->GetUnitInfo()->ZoomShadowMulti = Vars.Visuals.ScopeShadow;
				}

				if (ImGui::SliderFloat("Zoom Multiplier", &Vars.Visuals.ZoomMulti, 1.f, 100.f, "%.0f"))
				{
					G::LocalPlayer->GetUnit()->GetUnitInfo()->ZoomMultipler = Vars.Visuals.ZoomMulti;
					G::LocalPlayer->GetUnit()->GetUnitInfo()->ZoomCrosshairScale = Vars.Visuals.ZoomMulti; /*/ 2.f;*/
				}
			}
			ImGui::End();
		}

		if (Vars.Menu.Misc)
		{
			ImGui::Begin("Misc Settings", &Vars.Menu.Misc, ImVec2(230, 200), 0.9f);
			{
				if (ImGui::SliderFloat("Timescale (arcade)", &Vars.Misc.Timescale, 1.f, 100.f, "%.0f"))
					O::ClientView->timescale = Vars.Misc.Timescale;
				if (ImGui::Checkbox("Draw Penetration Crosshair", &Vars.Misc.DrawPenetrationIndicator))
					O::ClientView->PenetrationCrosshair = Vars.Misc.DrawPenetrationIndicator;
				if (ImGui::Checkbox("Draw Scope Distance", &Vars.Misc.DrawDistanceInScope))
					O::ClientView->DrawTankDistance = Vars.Misc.DrawDistanceInScope;

				if (ImGui::Checkbox("Unit Glow", &Vars.Misc.UnitGlow))
					O::ClientView->UnitGlow = Vars.Misc.UnitGlow;

				ImGui::Checkbox("Hide Box While Glow", &Vars.Misc.HideBoxWhileGlow);

				if (ImGui::Checkbox("Draw Plane Aim Crosshair", &Vars.Misc.DrawPlaneIndicator))
				{
					O::ClientView->PlaneAimIndicator = Vars.Misc.DrawPlaneIndicator;
					O::ClientView->PlaneAimIndicatorToPlane = Vars.Misc.DrawPlaneIndicator;
				}

				if (ImGui::Checkbox("Draw Plane Bomb Crosshair", &Vars.Misc.DrawBombIndicator))
					O::ClientView->DrawBombIndicator = Vars.Misc.DrawBombIndicator;

				if (ImGui::Checkbox("Arcade AutoTargeting", &Vars.Misc.ArcadeAutoTargeting))
				{
					O::ClientView->AutoTarget = Vars.Misc.ArcadeAutoTargeting;
					O::ClientView->AutoTargetLock = Vars.Misc.ArcadeAutoTargeting;
				}
			}
			ImGui::End();
		}

		if (Vars.Menu.Triggerbot)
		{
			ImGui::Begin("Triggerbot Settings", &Vars.Menu.Triggerbot, ImVec2(230, 200), 0.9f);
			{
				ImGui::Checkbox("Enable Triggerbot", &Vars.Triggerbot.Enabled);
				ImGui::Combo("Triggerbot Key", &Vars.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
				ImGui::BeginChild("Filter", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 6));
				{
					ImGui::Selectable("Green", &Vars.Triggerbot.Filter.Green);
					ImGui::Selectable("Yellow", &Vars.Triggerbot.Filter.Yellow);
					ImGui::Selectable("Red", &Vars.Triggerbot.Filter.Red);
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}

	Renderer::GetInstance()->BeginScene();
	if (G::HooksInitialized)
		Renderer::GetInstance()->DrawScene();
	Renderer::GetInstance()->EndScene();

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return D3D11Present_o(pSwapChain, SyncInterval, Flags);
}

void GUI_Init(HWND hWindow, ID3D11Device* pDevice, ID3D11DeviceContext *pContext)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.19f);
	colors[ImGuiCol_ChildWindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	//colors[ImGuiCol_ComboBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.99f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	//colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	//colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	//colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	//colors[ImGuiCol_CloseButton] = ImVec4(0.41f, 0.41f, 0.41f, 0.50f);
	//colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	//colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
#ifdef IMGUI_HAS_NAV
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
#endif

	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);
}