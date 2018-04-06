import os

Nact = 10000
Ntrj = 2000

PathBase = os.getcwd()

for it in range(1,6):
	os.system('mkdir %s/T%d' %(PathBase, it))
	Temperature = float(it) / 10

	for ib in range(-20,21):
		BiasField = float(ib) / 200

		JOBID = ('XE_T%d_X%d.sh' %(it, ib))

		os.system('mkdir %s/T%d/X%d' %(PathBase, it, ib))
		os.system('cp %s/main %s/T%d/X%d' %(PathBase, PathBase, it, ib))

		os.chdir('%s/T%d/X%d' %(PathBase, it, ib))

		os.system('echo "#!/bin/bash" > %s' %JOBID)
		os.system('echo "#PBS -l nodes=1:ppn=1" >> %s' %JOBID)
		os.system('echo "cd \$PBS_O_WORKDIR" >> %s' %JOBID)
		os.system('echo "date" >> %s' %JOBID)
		os.system('echo "./main tps %.8f %.8f %d %d" >> %s' %(Temperature, BiasField, Nact, Ntrj, JOBID))
		os.system('echo "date" >> %s' %JOBID)

		os.system('qsub %s' %JOBID)
		os.system('sleep 1')
