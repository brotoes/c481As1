#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <sys/time.h>

static void * gen_seg(void *);
static void * mult_seg(void *);

static pthread_barrier_t bar_gen;
static pthread_barrier_t bar_mult;
static int nproc;
static int mat_size;
static long * mat1;
static long * mat2;
static long * mat_fin;

struct seg_desc {
	int start;
	int end;
};

int main(int argc, char * argv[]) {
	struct seg_desc * segs;
	struct timeval start_fin;
	struct timeval end_fin;
	struct timeval init_fin;
	struct timeval gen_fin;
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
	int i;
	int seg_size;
	pthread_t * thread_ids;

	/*start timing*/
	gettimeofday(&start_fin, NULL);

	/*handle user input*/
	if (argc == 2) {
		nproc = strtol(argv[1], NULL, 10);
	} else if (argc == 3) {
		nproc = strtol(argv[1], NULL, 10);
		mat_size = strtol(argv[2], NULL, 10);
	} else {
		nproc = get_nprocs();
		mat_size = 1024;
	}

	segs = malloc(sizeof(struct seg_desc)*nproc);
	thread_ids = malloc(sizeof(pthread_t)*nproc);
	mat1 = malloc(sizeof(long)*mat_size*mat_size);
	mat2 = malloc(sizeof(long)*mat_size*mat_size);
	mat_fin = malloc(sizeof(long)*mat_size*mat_size);

	pthread_setconcurrency(nproc);

	/*Choose row assignments*/
	seg_size = mat_size/nproc;
	for (i = 0; i < nproc; i ++) {
		segs[i].start = i*seg_size;
		segs[i].end = (i + 1)*seg_size;
		if (i == nproc - 1) {
			segs[i].end += mat_size % nproc;
		}
	}

	gettimeofday(&init_fin, NULL);

	/*Generate matrices*/
	for (i = 0; i < nproc; i ++) {
		pthread_create(&thread_ids[i], NULL, &gen_seg, &segs[i]);
	}

	for (i = 0; i < nproc; i ++) {
		pthread_join(thread_ids[i], NULL);
	}

	gettimeofday(&gen_fin, NULL);

	/*Multiply Matrices*/
	for (i = 0; i < nproc; i ++) {
		pthread_create(&thread_ids[i], NULL, &mult_seg, &segs[i]);
	}

	for (i = 0; i < nproc; i ++) {
		pthread_join(thread_ids[i], NULL);
	}

	/*end timing*/
	gettimeofday(&end_fin, NULL);

	tot_sec = (int)end_fin.tv_sec - (int)start_fin.tv_sec;
	tot_usec = (int)end_fin.tv_usec - (int)start_fin.tv_usec;
	tot_time = (double)tot_sec + ((double)tot_usec/1000000.0);
	
	init_sec = (int)init_fin.tv_sec - (int)start_fin.tv_sec;
	init_usec = (int)init_fin.tv_usec - (int)start_fin.tv_usec;
	init_time = (double)init_sec + ((double)init_usec/1000000.0);

	gen_sec = (int)gen_fin.tv_sec - (int)init_fin.tv_sec;
	gen_usec = (int)gen_fin.tv_usec - (int)init_fin.tv_usec;
	gen_time = (double)gen_sec + ((double)gen_usec/1000000.0);

	mult_sec = (int)end_fin.tv_sec - (int)gen_fin.tv_sec;
	mult_usec = (int)end_fin.tv_usec - (int)gen_fin.tv_usec;
	mult_time = (double)mult_sec + ((double)mult_usec/1000000.0);
	
	printf("%lf %lf %lf %lf %d %d\n", tot_time, init_time, gen_time, 
	mult_time, nproc, mat_size);

	return 0;
}

/*generates a given segment of a matrix*/
static void * gen_seg(void * arg) {
	int row;
	int col;
	struct seg_desc seg = *((struct seg_desc*)arg);

	srandom(time(0));

	for (row = seg.start; row < seg.end; row ++) {
		for (col = 0; col < mat_size; col ++) {
			mat1[col + row*mat_size] = random();
			mat2[col + row*mat_size] = random();
		}
	}

	pthread_exit(0);
}

/*multiples a given segment of mat1 to the corresponding segment of mat2
 *and places the result in mat_fin
 */
static void * mult_seg(void * arg) {
	int row;
	int col;
	int cell;
	struct seg_desc seg = *((struct seg_desc*)arg);

	for (row = seg.start; row < seg.end; row ++) {
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

	pthread_exit(0);
}
