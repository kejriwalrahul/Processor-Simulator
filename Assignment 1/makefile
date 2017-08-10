# Builds the binaries
# Rahul Kejriwal, CS14B023

all: scenario1 scenario2

scenario1: mult_scenario_1.c
	gcc mult_scenario_1.c -std=c99 -O3 -o scenario1.o

scenario2: mult_scenario_2.c
	gcc mult_scenario_2.c -std=c99 -O3 -o scenario2.o

eval1:
	sudo perf stat -e cpu-cycles,instructions,branch-instructions,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,dTLB-loads,dTLB-load-misses,LLC-loads,LLC-load-misses -r 10 ./scenario1.o

eval2:
	sudo perf stat -e cpu-cycles,instructions,branch-instructions,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,dTLB-loads,dTLB-load-misses,LLC-loads,LLC-load-misses -r 10 ./scenario2.o

clean:
	rm -f *.o *.out