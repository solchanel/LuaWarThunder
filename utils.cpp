#include "main.hpp"
#include "utils.hpp"


namespace U
{
	SIZE_T GetPointer(SIZE_T Address, UINT Offset)
	{
		if (Address == 0)
			return 0;

		if (sizeof(SIZE_T) == 8)
			return Address + (INT)((*(INT*)(Address + Offset) + Offset) + sizeof(INT));

		return (SIZE_T)*(DWORD*)(Address + Offset);
	}
}