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

#include <sys/mman.h>
#define PROTECTION (PROT_READ | PROT_WRITE)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB)
#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40000
#endif

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
	for(i=0; i<assoc-1; i++)
		*((char**)(addr + i*W)) = (addr + (i+1)*W);
	
	*((char**)(addr + i*W)) = NULL;
}

unsigned long long repeatAccess(char *base_addr){
	unsigned long long tot_time = 0;
	for(int i=0; i<10000; i++){
		char *list_elem = base_addr;
		unsigned long long bef, aft;
		while(list_elem != NULL){
			bef = rdtscl();
			list_elem = *((char**)list_elem);
			aft = rdtscl();
			tot_time += aft-bef;
		}
	}

	return tot_time/10000;
}

void address_translation_test(){
	// char *base = malloc(2*S);
	char *base = mmap(NULL, 2*S, PROTECTION, FLAGS, 0, 0);
	if (base == MAP_FAILED) {
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
			temp_res = repeatAccess(base+set*B) / assoc;
			printf("%llu\t", temp_res);
		}
		printf("\n");
		assoc *= 2;
	}
}

int main(){
	A = 128;
	B = 64;
	N = 4096;
	W = B * N;
	S = A * W;

	address_translation_test();

	return 0;
}