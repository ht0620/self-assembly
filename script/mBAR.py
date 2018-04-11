from pymbar import MBAR
import os, sys, csv, numpy as np

Path = os.getcwd()

Nx = 41
Nd = 100
Nt = Nx * Nd
Ko = 1000

Trng = [float(idx) / 20 for idx in range(2, 10)]
Frng = [float(idx) / 2000 for idx in range(-20, 21)]

data = []

for Temp in Trng:
	t = []

	for Field in Frng:
		with open('%s/T%.4f/X%.4f/hist.csv' %(Path, Temp, Field), 'r') as hist:
			reader = csv.reader(hist)
			t.extend(map(float,[row[0] for row in reader]))

	xt = np.zeros((Nx, Nt))

	for ibias in range(Nx):
		Field = Frng[ibias]
		for itime in range(Nt):
			xt[ibias][itime] = t[itime] * Field * Ko

	Nk = [Nd] * Nx
	mbar = MBAR(xt, Nk)
	(Fij, Dij, Tij) = mbar.getFreeEnergyDifferences()

	data.append(Fij[20])

print data[0]

with open('result.txt', 'w') as res:
	for ibias in range(Nx):
		res.write('%.8f' %Frng[ibias])
		for itemp in range(8):
			res.write(' %.8f' %(- data[itemp][ibias] / Ko))
		res.write('\n')
