#include "memory.hpp"
#include <Psapi.h>
#include "../main.hpp"

#define NT_PROTECTVIRTUALMEMORY 0x50	// Win10
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define NtCurrentProcess() ((HANDLE)-1)

BOOL KeVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
	return NT_SUCCESS(Syscall<NTSTATUS>(NT_PROTECTVIRTUALMEMORY, NtCurrentProcess(), &lpAddress, &dwSize, PAGE_EXECUTE_READWRITE, lpflOldProtect)) ? TRUE : FALSE;
}

void Memory::Init()
{
	if (MH_Initialize() != MH_OK)
	{
		printf("[Memory::Init] MH_Initialize failed!\n");
		return;
	}
}
void Memory::CleanUp()
{
	MH_Uninitialize();
}

// =================================================================================
// Hook Function
// =================================================================================
bool Memory::HookFunction(DWORD64 pAddress, void* pDetour, void** ppOriginal, bool bKeVirtualProtect)
{
	// Create Hook
	int iResult = MH_CreateHook((void*)pAddress, pDetour, ppOriginal);
	if (iResult != MH_OK)
	{
		printf("[Memory::HookFunction] MH_CreateHook failed: %p [Error %i]\n", pAddress, iResult);
		MessageBox(0, "CreateHook fail", "Fail", 0);
		return false;
	}

	// Enable Hook
	iResult = bKeVirtualProtect ? MH_EnableHookEx((void*)pAddress, KeVirtualProtect) : MH_EnableHook((void*)pAddress);
	if (iResult != MH_OK)
	{
		printf("[Memory::HookFunction] MH_EnableHook failed: %p [Error %i]\n", pAddress, iResult);
		MessageBox(0, "EnableHook fail", "Fail", 0);
		return false;
	}

	// Success
	return true;
}
bool Memory::HookLibraryFunction(char* sLibrary, char* sName, void* pDetour, void** ppOriginal)
{
	// Module
	HMODULE hModule = GetModuleHandle(sLibrary);
	if (hModule == NULL)
	{
		printf("[Memory::HookLibraryFunction] Module %s not (yet) loaded!\n", sLibrary);
		return false;
	}

	// Proc
	void* pProc = GetProcAddress(hModule, sName);
	if (pProc == NULL)
	{
		printf("[Memory::HookLibraryFunction] Module %s has no exported function %s!\n", sLibrary, sName);
		return false;
	}

	// Hook
	return HookFunction((DWORD64)pProc, pDetour, ppOriginal, false);
}

// =================================================================================
// Copy
// =================================================================================
void Memory::Copy(DWORD64 pAddress, BYTE* bData, size_t stSize)
{
	DWORD dwOldProtection;
	VirtualProtect((void*)pAddress, stSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memcpy((void*)pAddress, (void*)bData, stSize);
	VirtualProtect((void*)pAddress, stSize, dwOldProtection, &dwOldProtection);
}

// =================================================================================
// Set
// =================================================================================
void Memory::Set(DWORD64 pAddress, BYTE* bData, size_t stSize)
{
	DWORD dwOldProtection;
	VirtualProtect((void*)pAddress, stSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memset((void*)pAddress, (int)bData, stSize);
	VirtualProtect((void*)pAddress, stSize, dwOldProtection, &dwOldProtection);
}

// =================================================================================
// FindPattern
// =================================================================================
bool Memory::Compare(const BYTE* pData, const BYTE* bMask, const char* sMask)
{
	for (; *sMask; ++sMask, ++pData, ++bMask)
		if (*sMask == 'x' && *pData != *bMask)
			return false;

	return *sMask == NULL;
}
DWORD64 Memory::Find(DWORD64 dwAddress, DWORD dwLength, const BYTE* bMask, const char* sMask)
{
	for (DWORD i = 0; i < dwLength; i++)
		if (Compare((BYTE*)(dwAddress + i), bMask, sMask))
			return (DWORD64)(dwAddress + i);

	return 0;
}

// =================================================================================
// Module Path
// =================================================================================
char* Memory::GetModulePath(HMODULE hModule)
{
	// Path
	char* path = new char[512];
	if (GetModuleFileName(hModule, path, 512) == 0)
		return NULL;

	// Return
	return path;
}
char* Memory::GetModulePath(char* sPath)
{
	// Module Handle
	HMODULE hModule = GetModuleHandle(sPath);
	if (hModule == NULL)
		return NULL;

	// Path
	return GetModulePath(hModule);
}

// =================================================================================
// Module Size
// =================================================================================
DWORD64 Memory::GetModuleSize(HMODULE hModule)
{
	// Double-Check
	if (hModule == NULL) return 0;

	// Module Info
	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(MODULEINFO));

	// Size
	return info.SizeOfImage;
}
