import sys
import matplotlib.pyplot as plt

# Config Vars
A = 12;
B = 64;
N = 4096;

# Usage Check
if len(sys.argv) != 2:
	print "Usage: python plot.py <data_fil>"
	sys.exit(1)

# Data file path
DATA_FILE = sys.argv[1]

# Read data
data = {}
last_assoc = 0
with open(DATA_FILE) as fp:
	for i, line in enumerate(fp):
		if i%2 == 0:
			last_assoc = int(line.strip())
			data[last_assoc] = None
		else:
			data[last_assoc] = line.strip().split('\t')


# plt.plot(range(N), data[1], label="1 assoc")
# plt.plot(range(N), data[2], label="2 assoc")
# plt.plot(range(N), data[4], label="4 assoc")
# plt.plot(range(N), data[8], label="8 assoc")
plt.plot(range(N), data[16], label="16 assoc")
plt.plot(range(N), data[32], label="32 assoc")
plt.plot(range(N), data[64], label="64 assoc")
plt.plot(range(N), data[128], label="128 assoc")
plt.xlabel("Set Number")
plt.ylabel("Latency")
plt.legend()
plt.show()