#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/time.h>

int main(int argc, char * argv[]) {
	int row;
	int col;
	int cell;
	int mat_size;
	long * mat1;
	long * mat2;
	long * mat_fin;
	struct timeval start_fin;
	struct timeval init_fin;
	struct timeval gen_fin;
	struct timeval end_fin;
	int init_sec;
	int init_usec;
	double init_time;
	int gen_sec;
	int gen_usec;
	double gen_time;
	int mult_sec;
	int mult_usec;
	double mult_time;
	int tot_sec;
	int tot_usec;
	double tot_time;

	/*start timing*/
	gettimeofday(&start_fin, NULL);

	if (argc == 2) {
		mat_size = strtol(argv[1], NULL, 10);
	} else {
		mat_size = 1024;
	}

	mat1 = malloc(sizeof(long)*mat_size*mat_size);
	mat2 = malloc(sizeof(long)*mat_size*mat_size);
	mat_fin = malloc(sizeof(long)*mat_size*mat_size);

	/*Generate matrices*/
	srandom(time(0));

	gettimeofday(&init_fin, NULL);
	
	for (row = 0; row < mat_size; row ++) {
		for (col = 0; col < mat_size; col ++) {
			mat1[col + row*mat_size] = random();
			mat2[col + row*mat_size] = random();
		}
	}

	gettimeofday(&gen_fin, NULL);
	
	/*Multiply Matrices*/
	for (row = 0; row < mat_size; row ++) {
		for (col = 0; col < mat_size; col ++) {
			int res_ind = row*mat_size + col;
			mat_fin[res_ind] = 0;
			for (cell = 0; cell < mat_size; cell ++) {
				int ind1 = row*mat_size + cell;
				int ind2 = cell*mat_size + col;
				long prod = mat1[ind1] * mat2[ind2];
				mat_fin[res_ind] += prod;
			}
		}
	}

	/*end timing*/
	gettimeofday(&end_fin, NULL);

	tot_sec = end_fin.tv_sec - start_fin.tv_sec;
	tot_usec = end_fin.tv_usec - start_fin.tv_usec;
	tot_time = (double)tot_sec + ((double)tot_usec/1000000.0);
	
	init_sec = init_fin.tv_sec - start_fin.tv_sec;
	init_usec = init_fin.tv_usec - start_fin.tv_usec;
	init_time = (double)init_sec + ((double)init_usec/1000000.0);

	gen_sec = gen_fin.tv_sec - init_fin.tv_sec;
	gen_usec = gen_fin.tv_usec - init_fin.tv_usec;
	gen_time = (double)gen_sec + ((double)gen_usec/1000000.0);
	
	mult_sec = end_fin.tv_sec - gen_fin.tv_sec;
	mult_usec = end_fin.tv_usec - gen_fin.tv_usec;
	mult_time = (double)mult_sec + ((double)mult_usec/1000000.0);
	
	printf("%lf %lf %lf %lf %d %d\n", tot_time, init_time, gen_time,
	mult_time, 1, mat_size);

	return 0;
}
