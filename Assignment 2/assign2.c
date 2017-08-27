/*
	Program to find LLC address translation logic in LLC 
	and cache replacement policy

	Rahul Kejriwal
	CS14B023
*/


/*
	Determining Cache Specs:
*/
// $ SYSNODE=/sys/devices/system/node
// $ grep '.*' $SYSNODE/node*/cpu*/cache/index*/* 2>/dev/null | 
// 		awk '-F[:/]' '{ printf "%6s %6s %24s %s\n" $6, $7, $9, $10, $11 ; }'

/*
	Huge Pages:
		mattwel@node-0:~$ sudo sysctl -w vm.nr_hugepages=2048
		vm.nr_hugepages = 2048
		mattwel@node-0:~$ cat /proc/meminfo | grep Huge
		HugePages_Total:    2048
		HugePages_Free:     2048
		HugePages_Rsvd:        0
		HugePages_Surp:        0
		Hugepagesize:       2048 kB
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <sys/mman.h>
#define PROTECTION (PROT_READ | PROT_WRITE)
#define MAP_HUGE_2MB    (21 << MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB    (30 << MAP_HUGE_SHIFT)
#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40000
#endif
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB)

int B, N, A;
int W, S;

unsigned long long rdtscl(void)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}

void init(char *addr, int assoc){
	int i;

	int ran_order[assoc];
	for(i=0; i< assoc; i++)
		ran_order[i] = i;

	srand(time(NULL));
	for(int i = assoc-1; i >= 0; --i){
		int j = rand() % (i+1);

		int temp = ran_order[i];
		ran_order[i] = ran_order[j];
		ran_order[j] = temp;
	}

	for(i=0; i<assoc-1; i++)
		*((char**)(addr + ran_order[i]*W)) = (addr + (ran_order[i+1])*W);
	
	*((char**)(addr + ran_order[i]*W)) = NULL;
}

unsigned long long repeatAccess(char *base_addr){
	int averaging_runs = 1000;
	int count = 0;

	unsigned long long tot_time = 0;
	for(int i=0; i<averaging_runs; i++){
		char *list_elem = base_addr;
		unsigned long long bef, aft;
		while(list_elem != NULL){
			bef = rdtscl();
			list_elem = *((char**)list_elem);
			aft = rdtscl();
			tot_time += aft-bef;
			count ++;
		}
	}

	return tot_time/count;
}

void address_translation_test(){
	// char *base = malloc(2*S);
	char *base = mmap(NULL, 50*S, PROTECTION, FLAGS, -1, 0);
	if(base == MAP_FAILED){
		fprintf(stderr, "#fail\n");
		perror("mmap");
		exit(1);
	}
 
	int assoc = 1, maxAssoc = 129;
	unsigned long long temp_res;
	while(assoc < maxAssoc){
		printf("%d\n", assoc);
		for(int set=0; set<N; set++){
			init(base+set*B, assoc);
			temp_res = repeatAccess(base+set*B);
			printf("%llu\t", temp_res);
		}
		printf("\n");
		assoc *= 2;
	}

	munmap(base, 50*S);
}

void double_scan(int high_assoc){
	char *base = mmap(NULL, 50*S, PROTECTION, FLAGS, -1, 0);
	if(base == MAP_FAILED){
		fprintf(stderr, "#fail\n");
		perror("mmap");
		exit(1);
	}
 
	unsigned long long temp_res;

	printf("F %d\n", high_assoc);
	for(int set=0; set<N; set++){
		init(base+set*B, high_assoc);
		temp_res = repeatAccess(base+set*B);
		printf("%llu\t", temp_res);
	}
	printf("\n");

	printf("B %d\n", high_assoc);
	for(int set=N-1; set>=0; set--){
		init(base+set*B, high_assoc);
		temp_res = repeatAccess(base+set*B);
		printf("%llu\t", temp_res);
	}
	printf("\n");

	munmap(base, 50*S);
}

int main(){
	A = 12;
	B = 64;
	N = 4096;
	W = B * N;
	S = A * W;

	address_translation_test();
	double_scan(64);
	return 0;
}