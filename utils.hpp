#pragma once
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

typedef enum {
	MOV = 3,
	LEA = 3,
	CMP = 3,
	CALL = 5,
} OFFSET_OP;

template<typename T>
class CPointer {
public:
	CPointer() { }
	~CPointer() { }

	CPointer(DWORD64 pointer) {
		p = (T **)pointer;
	}
	void Set(DWORD64 pointer) { p = (T **)pointer; }
	T *operator->() const { return (T *)*p; }
	bool operator !() const { return (T *)*p; }
	bool operator ==(T *o) const { return (o == (T *)*p); }

	T **p;
private:
};

namespace U
{
	SIZE_T GetPointer(SIZE_T Address, UINT Offset);

	template<typename T> __forceinline T GetOffsetX64(DWORD64 address, OFFSET_OP offset) {
		return (T)(address + (INT)((*(INT*)(address + offset) + offset) + sizeof(INT)));
	}

	template<typename T> T FindPattern(HMODULE hModule, std::string pattern, DWORD_PTR SizeOfImage = NULL)
	{
		const char* pat = pattern.c_str();
		DWORD_PTR firstMatch = 0;
		DWORD_PTR rangeStart = (T)hModule;
		MODULEINFO miModInfo;
		if (!SizeOfImage)
			GetModuleInformation(GetCurrentProcess(), hModule, &miModInfo, sizeof(MODULEINFO));
		DWORD_PTR rangeEnd = SizeOfImage ? rangeStart + SizeOfImage : rangeStart + (DWORD_PTR)miModInfo.SizeOfImage;
		for (DWORD_PTR pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return (T)firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return (T)firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;    //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}
}