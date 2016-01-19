#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "lab1_IO.h"
#include "timer.h"
#include<stdbool.h>


/*Define the maximum number of threads allowed to be created.*/
#define MAX_THREAD_COUNT 4096 

/*Apparently, matrices need to be global/shared so threads can access them*/
int **matrixA, **matrixB, **matrixC;
int n; /*Pointer to the row and/or column */

/*Need a struct to pass multiple values to the pthread_create function call*/
typedef struct thread_args {
	int row;
	int pid;
} thread_arg;

/*Function declaration*/
static bool isSquareNumber(long int number);
static bool evenlyDivides(long int number, int n);

/*The actual function called and run by created threads.
 * at this point, calculates the matrix multiplication for single point.*/
void *thread_one(void *arg_p){
	int x, k, l, m;
	thread_arg* t = (thread_arg*)arg_p;

	for(m = 0; m < n; m++){
		for(l = 0; l < t->row; l++){
			for(k = 0; k < n; k++){
				matrixC[l+(t->row * t->pid)][m] += matrixA[l+(t->row * t->pid)][k] * matrixB[k][m];
			}
		}
	}

	pthread_exit(&x);
}


/*Main function*/
int main(int argc, char **argv){
	
	/*Initialize some variables used in the program*/
	pthread_t myThreadHandle[MAX_THREAD_COUNT];
	int i=0, count;
	long int threadCount;
	double start=0.0, end=0.0;
	char *endptr;
	thread_arg **info;	

	if(argc == 2) threadCount = strtol(argv[1], &endptr, 10);
	else threadCount = 1;

	if(!isSquareNumber(threadCount) || !evenlyDivides(threadCount, n)){
		printf("According to the assignment description, you must pass a square number of threads\n
			And/or the number of threads must evenly divide into n^2.\n");
		return -1;	
	}

	/*Loads all data for matrices, and sets n value*/
	Lab1_loadinput(&matrixA, &matrixB, &n);

	/*Allocate space for the result of matrixA * matrixB*/
	matrixC = malloc(n * sizeof(int*));

	for(i = 0; i < n; i++){
		matrixC[i] = malloc(n * sizeof(int));
	}

	int r,c;
	for(r = 0; r < n; r++){
		for(c = 0; c < n; c++){
			matrixC[r][c] = 0;
		}
	}

	if(threadCount > MAX_THREAD_COUNT){
		printf("Sorry bro, too many threads. Max is %d\n", MAX_THREAD_COUNT);
		int m;
		for(m = 0; m < n; m++){
			free(matrixC[m]);
			free(matrixB[m]);
			free(matrixA[m]);
		}

		free(matrixA);
		free(matrixB);
		free(matrixC);

		exit(1);
	} else {
		
		long x;
		info = malloc(threadCount*sizeof(thread_arg*));

		for(x = 0; x < threadCount; x++){
			info[x] = malloc(n * sizeof(thread_arg));
		}
		
		/*Measurement of time, starting right before thread creation.*/
		GET_TIME(start);

		for(x = 0; x < threadCount; x++){
			/*Create the threads*/
			info[x]->row = n/threadCount;
			info[x]->pid = x;
			pthread_create(&myThreadHandle[x], NULL, &thread_one, (void *)info[x]);
		}
	
		for(count = 0; count < threadCount; count++){
			pthread_join(myThreadHandle[count], NULL);
		}

		/*Take ending time measurement*/
		GET_TIME(end);
	}	

	/*Saving result to a file. Both the answer, and the amount of time.*/
	Lab1_saveoutput(matrixC, &n, end-start);

	for(i = 0; i < threadCount; i++){
		free(info[i]);
	}	
	
	free(info);

	/*Freeing the mallocated memory.*/
	for(i = 0; i < n; i++){
		free(matrixC[i]);
		free(matrixA[i]);
		free(matrixB[i]);
	}

	free(matrixB);
	free(matrixA);
	free(matrixC);
	
	pthread_exit(NULL);
}

bool isSquareNumber(long int number){
	long int i;

	for(i = 0; i <= number; i++){
		if(number == i*i) return true;
	}

	return false;
}

bool evenlyDivides(long int number, int n){
		if(!(n*n % number)){
			return true;	
		}
		
		return false;
		}
}

}
