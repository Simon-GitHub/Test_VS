/*
	����malloc calloc realloc alloca
	�ٶȰٿ� realloc
*/
#include <malloc.h>

void test_memory_allocation()
{
	int * parr;
	parr = (int*)malloc(sizeof(int)*100);
	for(int i = 0; i < 100; ++i)
	{
		parr[i] = i;
	}
	free(parr);
}