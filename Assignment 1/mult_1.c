/*
	Program to multiplty 2 matrices A & B both in row-major order

	Rahul Kejriwal, CS14B023
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SIZE 1024

void main(){
	srand(time(NULL));
	struct timeval start, end;

	int *A[SIZE], *B[SIZE], *C[SIZE];
	for(int i=0; i<SIZE; i++){
		A[i] = malloc(sizeof(int)*SIZE);
		B[i] = malloc(sizeof(int)*SIZE);
		C[i] = malloc(sizeof(int)*SIZE);		
		
		for(int j=0; j<SIZE; j++){
			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;
		}
	}

	// Begin
	gettimeofday(&start, NULL);

	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			C[i][j] = 0;
			for(int k=0; k<SIZE; k++)
				C[i][j] += A[i][k]*B[k][j];
		}
	}

	// End
	gettimeofday(&end, NULL);

	printf("Elapsed Time(in milliseconds): %lf\n", (end.tv_sec - start.tv_sec)*1000 + ((end.tv_usec - start.tv_usec)*1.0)/1000);
}