import numpy as np
import subprocess
import sys
import os
import time
from shutil import copyfile

# n. burst times
n_bt = 100
n_at = 20
nfiles = n_bt * n_at
aver_values = []
for i in range(n_at):
	for j in range(n_bt):
		aver_values.append([i, j])
# use 100 processes
npr = 1000
# iterate over files
for i in range(nfiles):
	np.random.seed(i)
	# obtain processes
	processes = []
	aver = aver_values[i]
	at_aver = aver[0]
	bst_aver = aver[1]
	# set random seed
	at_min = 100
	for j in range(npr):
		idp = j
		prt = np.random.randint(0, 10)
		bst = abs(int(np.random.normal(bst_aver,  10, 1)))     # 10
		at  = abs(int(np.random.normal(j*at_aver, 2, 1)))      # 1
		if at < at_min:
			at_min = at
		processes.append([idp, at, bst, prt])
	process_list = []
	for j in range(npr):
		[idp, at, bst, prt] = processes[j]
		at = at - at_min
		if bst == 0:
			bst = 1
		process_list.append([idp, at, bst, prt])
	# build input file
	f = open("input", "w")
	for j in range(npr):
		[idp, at, bst, prt] = process_list[j]
		f.write("%d     " % idp + "%d     " % at + "%d     " % bst + "%d" % prt + "\n")
	f.close()
	copyfile("input", "input"+str(i+1))
	# append data on file
	f = open("data", "a")
	f.write("%d     " % at_aver + "%d" % bst_aver + "\n")
	f.close()
	# run the code
	p = subprocess.Popen(["./cpu_schedul", "input"])
	os.wait()
	time.sleep(1)
