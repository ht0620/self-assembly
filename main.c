#include "common.h"
#include "init.h"
#include "ctmc.h"
#include "debug.h"

int *rx, *ry;
int *dx, *dy;
int *hisx, *hisy;
int *ngrd, **npnt;
int *mgrd, **mpnt;
int *nb;
int Na, Ns;
double *ht;

uint32_t seed;

static void SaveConfig(int ia);
static void PrintBond(double t, FILE *hist);

int main(int argc, char *argv[])
{
	double T = atof(argv[1]);
	double X = atof(argv[2]);
	double B = 1/T;

	Na = atoi(argv[3]);
	Ns = atoi(argv[4]);

	seed = time(NULL);
	srand(time(NULL));

	AllocGlobal();
	AllocPointer();

	ClearLattice();
	RandomConfig();

	GlobalVarCTMC(B);
	ht[0] = InitialCTMC();
	SaveConfig(0);

	// Initial Trajectory
	for(int ia = 1; ia < Na; ia ++)
	{
		ht[ia] = UpdateCTMC();
		SaveConfig(ia);
	}

	FILE *hist;
	hist = fopen("hist.dat", "w");

	for(int is = 0; is < Ns; is ++)
	{
		double t = ShootingTPS(B, X);
		PrintBond(t, hist);
	}

	fclose(hist);

	free(rx);	free(ry);
	free(dx);	free(dy);
	free(hisx);	free(hisy);
	free(ngrd);	free(mgrd);
	free(npnt);	free(mpnt);
}

static void SaveConfig(int ia)
{
	for(int ip = 0; ip < Np; ip ++)
	{
		hx(ip, ia) = rx[ip];
		hy(ip, ia) = ry[ip];
	}
}

static void PrintBond(double t, FILE *hist)
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
