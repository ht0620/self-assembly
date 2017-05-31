#include "common.h"
#include "calc.h"
#include "rand.h"

#define tp(it, n)		iptmp[(it) + (n) * Nt]
#define td(it, n)		idtmp[(it) + (n) * Nt]

#define tit(id, ip)	ittmp[(id - 1) + (ip) * (Nd - 1)]
#define tnt(id, ip)	nttmp[(id - 1) + (ip) * (Nd - 1)]

static const int Nt = 7;
static const int Nm = 17;

static double *pt, *pc;
static int *nm, *nt, *dn;

static int *iptmp, *idtmp, *ittmp, *nttmp;

static void GetParticleNumber(int id, int ip);
static void MoveParticle(int id, int ip);

static void UpdatePtrs();
static void CalcPtrs();
static void CalcPcum();

static void GetTrans();
static double GetTime();

void GlobalVarCTMC(double B)
{
	pt = (double *) malloc(sizeof(double) * Nt);
	pc = (double *) malloc(sizeof(double) * Nt);

	nt = (int *) malloc(sizeof(int) * Nt);
	nm = (int *) malloc(sizeof(int) * Nm);

	iptmp = (int *) malloc(sizeof(int) * Nt * Np * (Nd - 1));
	idtmp = (int *) malloc(sizeof(int) * Nt * Np * (Nd - 1));
	ittmp = (int *) malloc(sizeof(int) * Np * (Nd - 1));
	nttmp = (int *) malloc(sizeof(int) * Np * (Nd - 1));

	for(int it = 0; it < Nt; it ++)
	{
		pt[it] = exp(B * (double) (it - 3));
		nt[it] = 0;

		if(pt[it] > 1.0)
		{
			pt[it] = 0.25;
		}

		else
		{
			pt[it] /= 4.0;
		}
	}
}

void InitialCTMC()
{
	double t = 0;

	CalcPtrs();
	CalcPcum();
	GetTrans();
	t += GetTime();

	for(int ip = 1; ip < Np; ip ++)
	{
		UpdatePtrs();
		CalcPcum();
		GetTrans();
		t += GetTime();
	}
}

double UpdateCTMC()
{
	double t = 0;

	for(int ip = 0; ip < Np; ip ++)
	{
		UpdatePtrs();
		CalcPcum();
		GetTrans();
		t += GetTime();
	}

	return t;
}

static void CalcPtrs()
{
	for(int it = 0; it < Nt; it ++)
	{
		nt[it] = 0;
	}

	for(int ip = 0; ip < Np; ip ++)
	{
		for(int id = 1; id < Nd; id ++)
		{
			if(np(id, 0, rx[ip], ry[ip]))
			{
				tit(id, ip) = -1;
			}

			else
			{
				int it = CalcNeighbor(id, ip) - CalcNeighbor(0, ip) + 2;

				tp(it, nt[it]) = ip;
				td(it, nt[it]) = id;

				tit(id, ip) = it;
				tnt(id, ip) = nt[it];

				nt[it] ++;
			}
		}
	}
}

static void UpdatePtrs()
{
	for(int im = 0; (0 <= nm[im]) && (im < Nm); im ++)
	{
		int ip = nm[im];

		for(int id = 1; id < Nd; id ++)
		{
			if(np(id, 0, rx[ip], ry[ip]))
			{
				if(-1 != tit(id, ip))
				{
					nt[tit(id, ip)] --;

					tp(tit(id, ip), tnt(id, ip)) = tp(tit(id, ip), nt[tit(id, ip)]);
					td(tit(id, ip), tnt(id, ip)) = td(tit(id, ip), nt[tit(id, ip)]);
					tnt(td(tit(id, ip), nt[tit(id, ip)]), tp(tit(id, ip), nt[tit(id, ip)])) = tnt(id, ip);

					tit(id, ip) = -1;
				}

				else
				{
					tit(id, ip) = -1;
				}
			}

			else
			{
				int it = tit(id, ip);
				int jt = CalcNeighbor(id, ip) - CalcNeighbor(0, ip) + 2;

				if(it != jt)
				{
					if(0 <= it)
					{
						nt[it] --;

						tp(it, tnt(id, ip)) = tp(it, nt[it]);
						td(it, tnt(id, ip)) = td(it, nt[it]);
						tnt(td(it, nt[it]), tp(it, nt[it])) = tnt(id, ip);
					}

					tp(jt, nt[jt]) = ip;
					td(jt, nt[jt]) = id;

					tit(id, ip) = jt;
					tnt(id, ip) = nt[jt];

					nt[jt] ++;
				}

				else
				{
					tit(id, ip) = jt;
				}
			}
		}
	}
}

static void CalcPcum()
{
	pc[0] = pt[0] * nt[0];

	for(int it = 1; it < Nt; it ++)
	{
		pc[it] = pt[it] * nt[it] + pc[it - 1];
	}
}

static void GetTrans()
{
	double u = XorDouble() * pc[Nt - 1];

	int it;

	for(it = 0; pc[it] < u; it ++);

	int mt = XorInteger() % nt[it];
	int ip = tp(it, mt);
	int id = td(it, mt);

	GetParticleNumber(id, ip);
	MoveParticle(id, ip);
}

static void GetParticleNumber(int id, int ip)
{
	for(int im = 0; im < Nm; im ++)
	{
		nm[im] = -1;
	}

	nm[0] = ip;

	int jd = (id + 1) % (Nd - 1) + 1;
	int im = 0;

	// Get particle number to update Pt, Pc
	for(int i = -2; i <= 0; i ++)
	{
		for(int j = - 2- i; j <= 2 + i; j ++)
		{
			if((0 != i) || (0 != j))
			{
				int x = mov(rx[ip], i*dx[id] + j*dx[jd]);
				int y = mov(ry[ip], i*dy[id] + j*dy[jd]);

				if(ng(x, y))
				{
					im ++;
					nm[im] = mg(x, y) - 1;
				}
			}
		}
	}

	for(int i = 1; i <= 3; i ++)
	{
		for(int j = -3 + i; j <= 3 - i; j ++)
		{
			if((1 != i) || (0 != j))
			{
				int x = mov(rx[ip], i*dx[id] + j*dx[jd]);
				int y = mov(ry[ip], i*dy[id] + j*dy[jd]);

				if(ng(x, y))
				{
					im ++;
					nm[im] = mg(x, y) - 1;
				}
			}
		}
	}
}

static double GetTime()
{
	return log(1 / XorDouble()) / pc[Nt - 1];
}

static void MoveParticle(int id, int ip)
{
	ng(rx[ip], ry[ip]) = 0;
	mg(rx[ip], ry[ip]) = 0;

	rx[ip] = mov(rx[ip], dx[id]);
	ry[ip] = mov(ry[ip], dy[id]);

	ng(rx[ip], ry[ip]) = 1;
	mg(rx[ip], ry[ip]) = ip + 1;
}
