#pragma once

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define NtCurrentProcess() ((HANDLE)-1)

typedef HRESULT WINAPI D3D11Present_t(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

extern D3D11_VIEWPORT viewport;
extern ID3D11Device *pDevice;
extern ID3D11DeviceContext *pContext;
extern D3D11Present_t *D3D11Present_o;

namespace H
{
	void Setup();
	bool D3D(HWND hGameWindow);
}