#include "common.h"
#include "rand.h"

void AllocGlobal()
{
	// allocate global variables
	rx = (int *) malloc(sizeof(int) * Np);
	ry = (int *) malloc(sizeof(int) * Np);

	hisx = (int *) malloc(sizeof(int) * Np * Na);
	hisy = (int *) malloc(sizeof(int) * Np * Na);

	dx = (int *) malloc(sizeof(int) * Nd);
	dy = (int *) malloc(sizeof(int) * Nd);

	nb = (int *) malloc(sizeof(int) * Nd);

	ngrd = (int *) malloc(sizeof(int) * Ng);
	mgrd = (int *) malloc(sizeof(int) * Ng);

	npnt = (int **) malloc(sizeof(int *) * Nv);
	mpnt = (int **) malloc(sizeof(int *) * Nv);

	ht = (double *) malloc(sizeof(double) * Na);

	// point array
	dx[0] = 0;	dx[1] = 1;	dx[2] =-1;	dx[3] = 0;	dx[4] = 0;
	dy[0] = 0;	dy[1] = 0;	dy[2] = 0;	dy[3] = 1;	dy[4] =-1;
}

void AllocPointer()
{
	for(int il = 0; il < Nl; il ++)
	{
		for(int jl = 0; jl < Nl; jl ++)
		{
			for(int id = 0; id < Nd; id ++)
			{
				for(int jd = 0; jd < Nd; jd ++)
				{
					int x = mov(il, dx[id] + dx[jd]);
					int y = mov(jl, dy[id] + dy[jd]);

					npnt[id + jd * Nd + il * Nd * Nd + jl * Nd * Nd * Nl] = &ng(x, y);
					mpnt[id + jd * Nd + il * Nd * Nd + jl * Nd * Nd * Nl] = &mg(x, y);
				}
			}
		}
	}
}

void ClearLattice()
{
	for(int il = 0; il < Nl; il ++)
	{
		for(int jl = 0; jl < Nl; jl ++)
		{
			ng(il, jl) = 0;
			mg(il, jl) = 0;
		}
	}
}

void RandomConfig()
{
	ClearLattice();

	// placing particle - infinite temperature
	for(int ip = 0; ip < Np ; ip ++)
	{
		do
		{
			rx[ip] = XorInteger() % Nl;
			ry[ip] = XorInteger() % Nl;
		}
		while( ng(rx[ip], ry[ip]) );

		ng(rx[ip], ry[ip]) = 1;
		mg(rx[ip], ry[ip]) = ip + 1;
	}
}
