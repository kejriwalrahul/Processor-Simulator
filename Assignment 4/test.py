import os
import subprocess

TEST_DIR = "Tests/"
OUT_DIR = "OUT/"

OUT2_DIR = "../Assignment 3/OUT/"

try:
	os.mkdir(OUT_DIR)
except:
	pass

files = os.listdir(TEST_DIR)
for file in files:
	with open(TEST_DIR+file) as fin, open(OUT_DIR+file,'w') as fout:
		subprocess.call('./simulate.sh', stdin=fin, stdout=fout)

	print 'diff ' + OUT_DIR + file + ' ' + OUT2_DIR + file
	out = subprocess.check_output(['diff', OUT_DIR + file, OUT2_DIR + file])
	if out == '':
		print "Success"
	else:
		print out
	