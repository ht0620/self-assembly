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

double *ht, *at, T;
double beta, bias;

uint32_t seed;

static void InitialTraj();
static void Xensemble();
static void FixedKobs();
static void FixedTobs();

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
		if(argc < 6)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too few arguments :(\n");
			printf("\n  Arguments: tps $(temperature) $(bias) $(K_obs) $(N_traj)\n\n");

			return 0;
		}

		if(argc > 6)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too many arguments :(\n");
			printf("\n  Arguments: tps $(temperature) $(bias) $(K_obs) $(N_traj)\n\n");

			return 0;
		}

		beta = 1 / atof(argv[2]);
		bias = atof(argv[3]);

		Na = atoi(argv[4]);
		Ns = atoi(argv[5]);

		hisx = (int *) malloc(sizeof(int) * Np * Na);
		hisy = (int *) malloc(sizeof(int) * Np * Na);

		ht = (double *) malloc(sizeof(double) * Na);
		at = (double *) malloc(sizeof(double) * Na);

		GlobalVarCTMC(beta);

		for(int i = 0; i < 100; i ++)
		{
			InitialTraj();
			Xensemble();
		}

		PrintTrajectory();


		free(hisx);
		free(hisy);
		free(ht);
		free(at);
	}

	else if(!strcmp(argv[1],"fxk"))
	{
		if(argc < 5)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too few arguments :(\n");
			printf("\n  Arguments: fxk $(temperature) $(K_obs) $(N_traj)\n\n");

			return 0;
		}

		if(argc > 5)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too many arguments :(\n");
			printf("\n  Arguments: fxk $(temperature) $(K_obs) $(N_traj)\n\n");

			return 0;
		}

		beta = 1 / atof(argv[2]);

		Na = atoi(argv[3]);
		Ns = atoi(argv[4]);

		GlobalVarCTMC(beta);

		FixedKobs();
	}


	else if(!strcmp(argv[1],"fxt"))
	{
		if(argc < 5)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too few arguments :(\n");
			printf("\n  Arguments: fxt $(temperature) $(t_obs) $(N_traj)\n\n");

			return 0;
		}

		if(argc > 5)
		{
			printf("\n  C R I T I C A L  E R R O R\n");
			printf("\n  Too many arguments :(\n");
			printf("\n  Arguments: fxt $(temperature) $(t_obs) $(N_traj)\n\n");

			return 0;
		}

		beta = 1 / atof(argv[2]);

		T = atof(argv[3]);
		Ns = atoi(argv[4]);

		GlobalVarCTMC(beta);

		FixedTobs();
	}

	else
	{
		printf("\n  C R I T I C A L  E R R O R\n");
		printf("\n  Wrong simulation type or the type has not specified :p\n");
		printf("\n  Simulation type: fxt fxk tps\n\n");
	}

	free(rx);	free(ry);
	free(dx);	free(dy);
	free(ngrd);	free(mgrd);
	free(npnt);	free(mpnt);

	return 0;
}

static void InitialTraj()
{
	ClearLattice();
	RandomConfig();

	ht[0] = InitialCTMC();
	SaveConfig(0);

	for(int ia = 1; ia < Na; ia ++)
	{
		ht[ia] = UpdateCTMC();
		at[ia] = ht[ia] / Ns;
		SaveConfig(ia);
	}
}

static void Xensemble()
{
	int nb[Nb];

	FILE *hist;
	hist = fopen("hist.csv", "a");

	for(int is = 0; is < Ns; is ++)
	{
		double t = ShootingTPS(QuenchBias(is, Ns));
		CalcBond(nb);

		if(is >= (Ns/2))
			PrintBond(t, nb, hist);

		for(int ia = 0; ia < Na; ia ++)
		{
			at[ia] += ht[ia] / Ns;
		}
	}

	fclose(hist);
}

static void FixedKobs()
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

	printf("%.8f,%d,%.8f,%.8f", 1/beta, Na, aobs, acum);

	for(int ib = 0; ib < Nb; ib ++)
	{
		printf(",%.8f", ab[ib]);
	}

	printf("\n");
}

static void FixedTobs()
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

	printf("%.8f,%.8f,%.8f,%.8f", 1/beta, T, aobs, acum);

	for(int ib = 0; ib < Nb; ib ++)
	{
		printf(",%.8f", ab[ib]);
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
