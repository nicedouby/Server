#pragma once
#include "stdlib.h"
class MemStat
{
	static void* S_malloc(int size)
	{
		static int SizeOfMallocedMem = 0;
		SizeOfMallocedMem = SizeOfMallocedMem + size;
		return malloc(size);
	}
	static void S_Free(void* p)
	{
		free(p);
	}
};

