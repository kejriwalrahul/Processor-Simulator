/*
	Program to multiplty 2 matrices A & B both in row-major order

	Rahul Kejriwal, CS14B023
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SIZE 1024

void matrix_mult(int *a, int *b, int *res, int m, int n, int p){
	for(int i=0; i<m; i++){
		for(int j=0; j<p; j++){
			res[i*SIZE+j] = 0;
			for(int k=0; k<n; k++)
				res[i*SIZE+j] += a[i*SIZE+k]*b[k*SIZE+j];
		}
	}
}

void fill_random(int *matrix, int m, int n){
	for(int i=0; i<m*n; i++)
		matrix[i] = rand() % 10;
}	

/*
void print_matrix(int *matrix, int m, int n){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++)
			printf("%d ", matrix[i*SIZE + j]);
		printf("\n");
	}
}
*/

void main(){
	srand(time(NULL));
	struct timeval start, end;

	int *A, *B, *C;
	A = malloc(sizeof(int)*SIZE*SIZE);
	B = malloc(sizeof(int)*SIZE*SIZE);
	C = malloc(sizeof(int)*SIZE*SIZE);

	fill_random(A, SIZE, SIZE);
	fill_random(B, SIZE, SIZE);

	// Begin
	gettimeofday(&start, NULL);

	matrix_mult(A, B, C, SIZE, SIZE, SIZE);

	// End
	gettimeofday(&end, NULL);

	printf("Elapsed Time(in milliseconds): %lf\n", (end.tv_sec - start.tv_sec)*1000 + ((end.tv_usec - start.tv_usec)*1.0)/1000);
}