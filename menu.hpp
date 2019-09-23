#pragma once

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GUI_Init(HWND hWindow, ID3D11Device* pDevice, ID3D11DeviceContext *pContext);
HRESULT WINAPI D3D11Present_h(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);