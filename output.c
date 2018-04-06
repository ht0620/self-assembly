#include "common.h"

void PrintBond(double t, int *nb, FILE *hist)
{
	fprintf(hist, "%8.8f,", t / Na);
	double sum = 0.0;

	for(int ib = 0; ib < Nd; ib ++)
	{
		fprintf(hist, "%0.8f,", (double)nb[ib] / Np);
		sum += (double)nb[ib] / Np;
	}

	fprintf(hist, "%0.8f\n", sum);
}

void PrintTrajectory()
{
	FILE *traj;
	FILE *thst;

	traj = fopen("traj.csv", "w");
	thst = fopen("thst.csv", "w");

	double htcum = 0;
	double atcum = 0;

	for(int ia = 0; ia < Na; ia ++)
	{
		htcum += ht[ia];
		atcum += at[ia];

		fprintf(traj, "%d,%.8f,%.8f\n", ia, ht[ia], htcum);
		fprintf(thst, "%d,%.8f,%.8f\n", ia, at[ia], atcum);

		for(int ip = 0; ip < Np; ip ++)
		{
			fprintf(traj, "%d,%d,%d\n", ip, hx(ip, ia), hy(ip, ia));
		}
	}

	fclose(traj);
	fclose(thst);
}
