#include "common.h"

void DebugPosition()
{
	int ip;

	for(ip = 0; ip < Np; ip ++)
	{
		if((rx[ip] >= Nl) || (0 > rx[ip]))
		{
			printf("Pos x ERROR\n");
		}

		if((ry[ip] >= Nl) || (0 > ry[ip]))
		{
			printf("Pos y ERROR\n");
		}
	}
}

void DebugLattice()
{
	for(int ip = 0; ip < Np; ip ++)
	{
		if(1 != ng(rx[ip], ry[ip]))
		{
			printf("ERROR1 on ngrid\n");
		}

		if(ip+1 != mg(rx[ip], ry[ip]))
		{
			printf("ERROR1 on mgrid\n");
		}
	}

	int s1 = 0;
	int s2 = 0;

	for(int jl = 0; jl < Nl; jl ++)
	{
		for(int il = 0; il < Nl; il ++)
		{
			s1 += ng(il, jl);
			s2 += mg(il, jl);
		}
	}

//	printf("%d\t%d\n", s1, s2);

}

void DebugGrid()
{
	int il, jl;
	int n = 0;
	int m = 0;

	for(jl = 0; jl < Nl; jl ++)
	{
		for(il = 0; il < Nl; il ++)
		{
			if(0 == ng(il, jl))
				printf(" - ");

			else
				printf(" O ");

			n += ng(il, jl);
		}

		printf("\t");

		for(il = 0; il < Nl; il ++)
		{
			if(0 == mg(il, jl))
				printf(" - ");

			else
				printf(" %d ", mg(il, jl) - 1);

			m += mg(il, jl);
		}

		printf("\n");
	}

	printf("N = %d, M = %d\n\n", n, m);
}
