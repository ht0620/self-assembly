#include "common.h"
#include "output.h"
#include "debug.h"
#include "init.h"
#include "calc.h"
#include "ctmc.h"
#include "tps.h"

int *rx, *ry;
int *dx, *dy;
int *hisx, *hisy;
int *ngrd, **npnt;
int *mgrd, **mpnt;
int Na, Ns;

double *ht, T;
double beta, bias;

uint32_t seed;

static inline void InitialTraj();
static inline void Xensemble();
static inline void FixedK();
static inline void FixedT();

static void SaveConfig(int ia);
static double QuenchBias(int is, int Ns);

int main(int argc, char *argv[])
{
	seed = time(NULL);
	srand(time(NULL));

	AllocGlobal();
	AllocPointer();

	if(!strcmp(argv[1],"tps"))
	{
		beta = 1 / atof(argv[2]);
		bias = atof(argv[3]);

		Na = atoi(argv[4]);
		Ns = atoi(argv[5]);

		hisx = (int *) malloc(sizeof(int) * Np * Na);
		hisy = (int *) malloc(sizeof(int) * Np * Na);
		ht = (double *) malloc(sizeof(double) * Na);

		GlobalVarCTMC(beta);

		InitialTraj();
		Xensemble();

		free(hisx);
		free(hisy);
		free(ht);
	}

	else if(!strcmp(argv[1],"fxk"))
	{
		beta = 1 / atof(argv[2]);

		Na = atoi(argv[3]);
		Ns = atoi(argv[4]);

		GlobalVarCTMC(beta);

		FixedK();
	}


	else if(!strcmp(argv[1],"fxt"))
	{
		beta = 1 / atof(argv[2]);

		T = atof(argv[3]);
		Ns = atoi(argv[4]);

		GlobalVarCTMC(beta);

		FixedT();
	}

	else
	{
		printf("#################################################################\n");
		printf("## ERROR! Wrong simulation type of the type has not specified. ##\n");
		printf("#################################################################\n");
	}

	free(rx);	free(ry);
	free(dx);	free(dy);
	free(ngrd);	free(mgrd);
	free(npnt);	free(mpnt);

	return 0;
}

static inline void InitialTraj()
{
	ClearLattice();
	RandomConfig();

	ht[0] = InitialCTMC();
	SaveConfig(0);

	for(int ia = 1; ia < Na; ia ++)
	{
		ht[ia] = UpdateCTMC();
		SaveConfig(ia);
	}
}

static inline void Xensemble()
{
	int nb[Nb];

	FILE *hist;
	hist = fopen("hist.dat", "w");

	for(int is = 0; is < Ns; is ++)
	{
		double t = ShootingTPS(QuenchBias(is, Ns));
		CalcBond(nb);
		PrintBond(t, nb, hist);
	}

	fclose(hist);
}

static inline void FixedK()
{
	double aobs = 0;
	double acum = 0;
	double ab[Nb] = {0, 0, 0, 0, 0};

	int nb[Nb];

	for(int is = 0; is < Ns; is ++)
	{
		ClearLattice();
		RandomConfig();

		double tobs = InitialCTMC();
		double tcum = tobs;

		for(int ia = 1; ia < Na; ia ++)
		{
			tobs = UpdateCTMC();
			tcum += tobs;
		}

		CalcBond(nb);

		aobs += tobs / Ns;
		acum += tcum / Ns / Na;

		for(int ib = 0; ib < Nb; ib ++)
		{
			ab[ib] += (double) nb[ib] / Np / Ns;
		}
	}

	printf("%.8f\t%d\t%.8f\t%.8f", 1/beta, Na, aobs, acum);

	for(int ib = 0; ib < Nb; ib ++)
	{
		printf("\t%.8f", ab[ib]);
	}

	printf("\n");
}

static inline void FixedT()
{
	double aobs = 0;
	double acum = 0;
	double ab[Nb] = {0, 0, 0, 0, 0};

	int nb[Nb];

	for(int is = 0; is < Ns; is ++)
	{
		ClearLattice();
		RandomConfig();

		double tobs = InitialCTMC();
		double tcum = tobs;

		Na = 1;

		while(tcum < T)
		{
			tobs = UpdateCTMC();
			tcum += tobs;

			Na ++;
		}

		aobs += tobs / Ns;
		acum += tcum / Ns / Na;

		CalcBond(nb);

		for(int ib = 0; ib < Nb; ib ++)
		{
			ab[ib] += (double) nb[ib] / Np / Ns;
		}
	}

	printf("%.8f\t%.8f\t%.8f\t%.8f", 1/beta, T, aobs, acum);

	for(int ib = 0; ib < Nb; ib ++)
	{
		printf("\t%.8f", ab[ib]);
	}

	printf("\n");
}

static void SaveConfig(int ia)
{
	for(int ip = 0; ip < Np; ip ++)
	{
		hx(ip, ia) = rx[ip];
		hy(ip, ia) = ry[ip];
	}
}

static double QuenchBias(int is, int Ns)
{
	if(is < (Ns / 10))
	{
		return 10 * bias * is / Ns;
	}

	else
	{
		return bias;
	}
}
