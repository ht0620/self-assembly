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
	FILE *thst;

	traj = fopen("traj.dat", "w");
	thst = fopen("thst.dat", "w");

	double htcum = 0;
	double atcum = 0;

	for(int ia = 0; ia < Na; ia ++)
	{
		htcum += ht[ia];
		atcum += at[ia];

		fprintf(traj, "%d\t%.8f\t%.8f\n", ia, ht[ia], htcum);
		fprintf(thst, "%d\t%.8f\t%.8f\n", ia, at[ia], atcum);

		for(int ip = 0; ip < Np; ip ++)
		{
			fprintf(traj, "%d\t%d\t%d\n", ip, hx(ip, ia), hy(ip, ia));
		}
	}

	fclose(traj);
	fclose(thst);
}
