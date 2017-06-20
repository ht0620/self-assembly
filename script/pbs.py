import os

Nactv = 10000
Ntraj = 100000

inode = 16
jnode = 17

pbase = os.getcwd()
pplus = 'plus'
pmins = 'mins'

itemp = 15
os.system('mkdir %s/%d' %(pbase, itemp))
temp = float(itemp) / 100

for ibias in range(1,17):
	os.system('mkdir %s/%d/%d' %(pbase, itemp, ibias))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pplus))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pmins))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pplus))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pmins))

	bias = 2 * float(ibias) / 1000000
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pplus))
	os.system('echo "#!/bin/bash" > plus.sh')
	os.system('echo "#PBS -l nodes=node%d" >> plus.sh' %inode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> plus.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> plus.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub plus.sh')
	os.system('sleep 1')

	bias = - bias
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pmins))
	os.system('echo "#!/bin/bash" > mins.sh')
	os.system('echo "#PBS -l nodes=node%d" >> mins.sh' %jnode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> mins.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> mins.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub mins.sh')
	os.system('sleep 1')

inode = inode + 2
jnode = jnode + 2

itemp = 25
os.system('mkdir %s/%d' %(pbase, itemp))
temp = float(itemp) / 100

for ibias in range(1,17):
	os.system('mkdir %s/%d/%d' %(pbase, itemp, ibias))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pplus))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pmins))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pplus))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pmins))

	bias = 2 * float(ibias) / 100000
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pplus))
	os.system('echo "#!/bin/bash" > plus.sh')
	os.system('echo "#PBS -l nodes=node%d" >> plus.sh' %inode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> plus.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> plus.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub plus.sh')
	os.system('sleep 1')

	bias = - bias
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pmins))
	os.system('echo "#!/bin/bash" > mins.sh')
	os.system('echo "#PBS -l nodes=node%d" >> mins.sh' %jnode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> mins.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> mins.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub mins.sh')
	os.system('sleep 1')

inode = inode + 2
jnode = jnode + 2

itemp = 45
os.system('mkdir %s/%d' %(pbase, itemp))
temp = float(itemp) / 100

for ibias in range(1,17):
	os.system('mkdir %s/%d/%d' %(pbase, itemp, ibias))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pplus))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pmins))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pplus))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pmins))

	bias = float(ibias) / 1000
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pplus))
	os.system('echo "#!/bin/bash" > plus.sh')
	os.system('echo "#PBS -l nodes=node%d" >> plus.sh' %inode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> plus.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> plus.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub plus.sh')
	os.system('sleep 1')

	bias = - bias
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pmins))
	os.system('echo "#!/bin/bash" > mins.sh')
	os.system('echo "#PBS -l nodes=node%d" >> mins.sh' %jnode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> mins.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> mins.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub mins.sh')
	os.system('sleep 1')

inode = inode + 2
jnode = jnode + 2

itemp = 55
os.system('mkdir %s/%d' %(pbase, itemp))
temp = float(itemp) / 100

for ibias in range(1,17):
	os.system('mkdir %s/%d/%d' %(pbase, itemp, ibias))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pplus))
	os.system('mkdir %s/%d/%d/%s' %(pbase, itemp, ibias, pmins))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pplus))
	os.system('cp %s/main %s/%d/%d/%s' %(pbase, pbase, itemp, ibias, pmins))

	bias = 2 * float(ibias) / 1000
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pplus))
	os.system('echo "#!/bin/bash" > plus.sh')
	os.system('echo "#PBS -l nodes=node%d" >> plus.sh' %inode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> plus.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> plus.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub plus.sh')
	os.system('sleep 1')

	bias = - bias
	os.chdir('%s/%d/%d/%s' %(pbase,itemp,ibias,pmins))
	os.system('echo "#!/bin/bash" > mins.sh')
	os.system('echo "#PBS -l nodes=node%d" >> mins.sh' %jnode)
	os.system('echo "cd \$PBS_O_WORKDIR" >> mins.sh')
	os.system('echo "./main tps %.8f %.8f %d %d" >> mins.sh' %(temp, bias, Nactv, Ntraj))
	os.system('qsub mins.sh')
	os.system('sleep 1')
