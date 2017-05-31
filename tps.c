#include "common.h"
#include "ctmc.h"
#include "init.h"
#include "rand.h"
#include "calc.h"

#define tx(i, j)	tmpx[(i) + (j)*Np]
#define ty(i, j)	tmpy[(i) + (j)*Np]

static void SaveConfig(int ia);
static void LoadConfig(int M);
static void AcceptTraj();
static void RejectTraj();
static void QuenchBias(double *x, double X);
static double CalcTime();
static double CalcTimeDelta();

static int *tmpx, *tmpy, Ma;
static double *tt;
static double x = 0;

double ShootingTPS(double B, double X)
{
	Ma = XorInteger() % (Na - 1) + 1;

	tmpx = (int *)malloc(sizeof(int) * Np * Ma);
	tmpy = (int *)malloc(sizeof(int) * Np * Ma);
	tt = (double *)malloc(sizeof(double) * Ma);

 	LoadConfig(Ma);

	tt[0] = InitialCTMC(B);
	SaveConfig(0);

	for(int ia = 1; ia < Ma; ia ++)
	{
		tt[ia] = UpdateCTMC(B);
		SaveConfig(ia);
	}

	double dt = CalcTimeDelta();

	double r = XorDouble();
	double p = exp(-x * dt);

	if(r < p)
	{
		AcceptTraj(Ma);
		QuenchBias(&x, X);
	}

	else
	{
		RejectTraj();
	}

	free(tmpx);
	free(tmpy);
	free(tt);

	CalcBond();
	double t = CalcTime();

	return t;
}

static void QuenchBias(double *x, double X)
{
	if((X > 0.0) && (X > *x))
	{
		*x += X / 100;
	}

	if((X < 0.0) && (X < *x))
	{
		*x += X / 100;
	}
}

static void AcceptTraj()
{
	for(int ia = 0; ia < Ma; ia ++)
	{
		for(int ip = 0; ip < Np; ip ++)
		{
			hx(ip, ia + Na - Ma) = tx(ip, ia);
			hy(ip, ia + Na - Ma) = ty(ip, ia);
		}

		ht[ia + Na - Ma] = tt[ia];
	}
}

static void RejectTraj()
{
	LoadConfig(0);
	CalcBond();
}

static double CalcTime()
{
	double t = 0;

	for(int ia = 0; ia < Na; ia ++)
	{
		t += ht[ia];
	}

	return t;
}

static double CalcTimeDelta()
{
	double dt = 0;

	for(int ia = 0; ia < Ma; ia ++)
	{
		dt += tt[ia];
		dt -= ht[ia + Na - Ma];
	}

	return dt;
}

static void SaveConfig(int ia)
{
	for(int ip = 0; ip < Np; ip ++)
	{
		tx(ip, ia) = rx[ip];
		ty(ip, ia) = ry[ip];
	}
}

static void LoadConfig(int M)
{
	ClearLattice();

	for(int ip = 0; ip < Np; ip ++)
	{
		rx[ip] = hx(ip, Na - M - 1);
		ry[ip] = hy(ip, Na - M - 1);

		ng(rx[ip], ry[ip]) = 1;
		mg(rx[ip], ry[ip]) = ip + 1;
	}
}
