import sys
import subprocess
import matplotlib.pyplot as plt

# Config Vars
A = 12;
B = 64;
N = 4096;

# Usage Check
"""
if len(sys.argv) != 2:
	print "Usage: python plot.py <data_fil>"
	sys.exit(1)
"""

# Run the code
print "Running Code for Tests"
with open("outfile",'w') as fp:
	subprocess.call(['./assign2.o'], stdout=fp)
print "Done!"

# Data file path
# DATA_FILE = sys.argv[1]
DATA_FILE = "outfile"

# Read data
data = {}
last_assoc = 0
with open(DATA_FILE) as fp:
	for i, line in enumerate(fp):
		if i%2 == 0:
			if line.startswith('F'):
				data['F'] = None
				last_assoc = 'F'
			elif line.startswith('B'):
				data['B'] = None
				last_assoc = 'B'
			else:
				last_assoc = int(line.strip())
				data[last_assoc] = None
		else:
			data[last_assoc] = line.strip().split('\t')

plot_assoc_vals = [4, 8 , 16, 32, 64, 128]
for i in plot_assoc_vals:
	plt.plot(range(N), data[i], label=str(i) + " assoc")
	
plt.xlabel("Set Number")
plt.ylabel("Latency")
plt.legend()
plt.show()

plt.plot(range(N), data['F'], label="Forward Scan")
rev_range = range(N);
rev_range.reverse()
plt.plot(rev_range, data['B'], label="Backward Scan")
plt.xlabel("Set Number")
plt.ylabel("Latency")
plt.legend()
plt.show()