#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct {
    int i;
    int** a;
    int** b;
    int** c;
    int len;
} mat_mul_arg_t;

typedef struct {
    int i;
    int** a;
    int** b;
    int** c;
    int len;
    int core;
    int core_index;
} mat_mul_arg_t_hw;

void initialize (int*** p_a, int*** p_b, int*** p_c, int len);
void printMatrix (int** a, int** b, int** c, int len);

void matrixAddition (int** a, int** b, int** c, int len);
void matrixSubtraction (int** a, int** b, int** c, int len);
void matrixMultiplication (int** a, int** b, int ** c, int len);

void *matrixMultiplicationThreadKernel(void *p_arg);
void matrixMultiplicationThread (int** a, int** b, int **c, int len);
void *matrixMultiplicationHWThreadKernel(void *p_arg);
void matrixMultiplicationHWThread (int** a, int** b, int **c, int len, int num_thread);

void invertMatrix (int** matrix, int len);

double wtime();