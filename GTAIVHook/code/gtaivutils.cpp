#include "gtaivutils.h"
#include <Windows.h>
int GetMKXEntryPoint()
{
	static int addr = reinterpret_cast<int>(GetModuleHandle(nullptr));
	return addr;
}

int _ivaddr(int addr)
{
	return GetMKXEntryPoint() - 0x400000 + addr;
}
