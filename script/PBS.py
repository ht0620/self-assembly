import os, sys

Nact = int(sys.argv[1])
Ntrj = int(sys.argv[2])

Trng = [float(idx) / 10 for idx in range(1,5)]
Frng = [float(idx) / 2000 for idx in range(-20, 21)]

Path = os.getcwd()

for Temp in Trng:
	os.system('mkdir %s/T%.4f' %(Path, Temp))

	for Field in Frng:
		JobID = ('XE_T%.4f_%.4f.sh' %(Temp, Field))
		os.system('mkdir %s/T%.4f/X%.4f' %(Path, Temp, Field))
		os.system('cp %s/main %s/T%.4f/X%.4f' %(Path, Path, Temp, Field))
		os.chdir('%s/T%.4f/X%.4f' %(Path, Temp, Field))

		with open(JobID, 'w') as PBS:
			PBS.write('#!/bin/bash\n')
			PBS.write('#PBS -l nodes=1:ppn=1\n')
			PBS.write('cd $PBS_O_WORKDIR\n')
			PBS.write('date\n')
			PBS.write('rm *.csv\n')
			PBS.write('./main tps %.6f %.6f %d %d\n' %(Temp, Field, Nact, Ntrj))
			PBS.write('date')

		os.system('qsub %s' %JobID)
		os.system('sleep 1')

