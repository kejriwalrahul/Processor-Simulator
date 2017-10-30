import os
import subprocess

TEST_DIR = "Tests/"
OUT_DIR = "OUT/"

try:
	os.mkdir(OUT_DIR)
except:
	pass

files = os.listdir(TEST_DIR)
for file in files:
	with open(TEST_DIR+file) as fin, open(OUT_DIR+file,'w') as fout:
		subprocess.call('./simulate.sh', stdin=fin, stdout=fout)