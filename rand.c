#include "common.h"

extern uint32_t seed;

uint32_t XorInteger()
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;

	return seed;
}

double XorDouble()
{
	return (double) XorInteger() / 0xffffffff;
}
