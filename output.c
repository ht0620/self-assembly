#include "common.h"

void PrintBond(double t, int *nb, FILE *hist)
{
	fprintf(hist, "%8.8f\t", t / Na);
	double sum = 0.0;

	for(int ib = 0; ib < Nd; ib ++)
	{
		fprintf(hist, "%0.8f\t", (double)nb[ib] / Np);
		sum += (double)nb[ib] / Np;
	}

	fprintf(hist, "%0.8f\n", sum);
}

void PrintTrajectory()
{
	FILE *traj;
	traj = fopen("traj.dat", "w");

	for(int ia = 0; ia < Na; ia ++)
	{
		fprintf(traj, "%.8f\n", ht[ia]);

		for(int ip = 0; ip < Np; ip ++)
		{
			fprintf(traj, "%d\t%d\t%d\n", ip, hx(ip, ia), hy(ip, ia));
		}
	}

	fclose(traj);
}
