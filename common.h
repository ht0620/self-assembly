#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define sqr(i, j)		(i)*(i) + (j)*(j)
#define mov(i, j)		((i)+(j)+Nl) % Nl

#define ng(i, j)		ngrd[(i) + (j) * Nl]
#define mg(i, j)		mgrd[(i) + (j) * Nl]

#define np(i, j, k, l)	*npnt[(i) + (j) * Nd + (k) * Nd * Nd + (l) * Nd * Nd * Nl]
#define mp(i, j, k, l)	*mpnt[(i) + (j) * Nd + (k) * Nd * Nd + (l) * Nd * Nd * Nl]

//#define np(i, j, k, l)	ng(mov(dx[i] + dx[j], k), mov(dy[i] + dy[j], l))
//#define mp(i, j, k, l)	mg(mov(dx[i] + dy[i], k), mov(dy[i] + dy[j], l))

#define hx(i, j)		hisx[(i) + (j) * Np]
#define hy(i, j)		hisy[(i) + (j) * Np]

/// Constants
static const int Nl = 72;
static const int Np = 512;
static const int Nd = 5;
static const int Nb = 5;

static const int Ng = Nl * Nl;
static const int Nv = Nl * Nl * Nd * Nd;

/// Variables
extern int Na;

extern int *rx, *ry;
extern int *dx, *dy;

extern int *hisx, *hisy;
extern double *ht;

extern int *ngrd, **npnt;
extern int *mgrd, **mpnt;

extern int *nb;

/// TPS scheme
double ShootingTPS(double B, double X);

#endif
