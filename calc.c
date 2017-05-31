#include "common.h"

int CalcNeighbor(int id, int ip)
{
	int ib = 0;

	for(int jd = 1; jd < Nd; jd ++)
	{
		ib += np(id, jd, rx[ip], ry[ip]);
	}

	return ib;
}

void CalcBond()
{
	for(int ib = 0; ib < Nb; ib ++)
	{
		nb[ib] = 0;
	}

	for(int ip = 0; ip < Np; ip ++)
	{
		int ib = CalcNeighbor(0, ip);
		nb[ib] ++;
	}
}
