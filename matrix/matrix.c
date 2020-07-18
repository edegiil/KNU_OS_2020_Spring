#include "matrix.h"

void initialize (int*** p_a, int*** p_b, int*** p_c, int len)
{
    int **a;
    int **b;
    int **c;
    
    a = (int**)malloc(len * sizeof(int*));
    b = (int**)malloc(len * sizeof(int*));
    c = (int**)malloc(len * sizeof(int*));

    for (int i = 0; i < len; i++) { 
        a[i] = (int*)malloc(len * sizeof(int));
        b[i] = (int*)malloc(len * sizeof(int));
        c[i] = (int*)malloc(len * sizeof(int));
    }


    for (int i = 0; i < len; i++) { 
        for (int j = 0; j < len; j++) { 
            a[i][j] = random() % 10;
            b[i][j] = random() % 10;
            c[i][j] = 0;
        }
    }

    *p_a = a;
    *p_b = b;
    *p_c = c;
}

void printMatrix (int** a, int** b, int** c, int len)
{
    printf("===== Matrix A =====\n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }

    printf("===== Matrix B =====\n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d\t", b[i][j]);
        }
        printf("\n");
    }

    printf("===== Matrix C =====\n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d\t", c[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

void matrixAddition (int** a, int** b, int** c, int len)
{
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            c[i][j] = a[i][j] + b[i][j]; 
        }
    }
}

void matrixSubtraction (int** a, int** b, int** c, int len)
{
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            c[i][j] = a[i][j] - b[i][j]; 
        }
    }
}

void matrixMultiplication (int** a, int** b, int **c, int len)
{
    for (int i = 0; i < len; i++) {  // row
        for (int j = 0; j < len; j++) {  // column
            c[i][j] = 0;
            for (int k = 0; k < len; k++) { 
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void *matrixMultiplicationThreadKernel(void *p_arg)
{
    mat_mul_arg_t *arg = (mat_mul_arg_t*)p_arg;
    int i = arg->i;
    int** a = arg->a;
    int** b = arg->b;
    int** c = arg->c;
    int len = arg->len;

    for (int j = 0; j < len; j++) {
        c[i][j] = 0;
        for (int k = 0; k < len; k++) {
            c[i][j] += a[i][k] * b[k][j];
        }
    }
}

void matrixMultiplicationThread (int** a, int** b, int **c, int len)
{
    int res;
    mat_mul_arg_t *arg;
    pthread_t *a_thread;
    void *thread_result;

    a_thread = (pthread_t*)malloc(sizeof(pthread_t) * len);

    for (int i = 0; i < len; i++) {
        arg = (mat_mul_arg_t*)malloc(sizeof(mat_mul_arg_t));
        arg->i = i;
        arg->a = a;
        arg->b = b;
        arg->c = c;
        arg->len = len;

        res = pthread_create(a_thread+i, NULL, matrixMultiplicationThreadKernel, (void*)arg);
        if (res) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < len; i++) {
        res = pthread_join(a_thread[i], &thread_result);
        if (res) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }
}

void *matrixMultiplicationHWThreadKernel(void *p_arg)
{
    mat_mul_arg_t_hw *arg = (mat_mul_arg_t_hw*)p_arg;
    int i = arg->i;
    int** a = arg->a;
    int** b = arg->b;
    int** c = arg->c;
    int len = arg->len;
    int core = arg->core;

    for (int l = i; l < len; l += core) {
        for (int j = 0; j < len; j++) {
            c[l][j] = 0;
            for (int k = 0; k < len; k++) {
                c[l][j] += a[l][k] * b[k][j];
            }
        }
    }
}

void matrixMultiplicationHWThread (int** a, int** b, int **c, int len, int num_thread)
{
    int res;
    mat_mul_arg_t_hw *arg;
    pthread_t *a_thread;
    void *thread_result;

    a_thread = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);


    for (int i = 0; i < num_thread; i++) {
        arg = (mat_mul_arg_t_hw*)malloc(sizeof(mat_mul_arg_t_hw));
        arg->i = i;
        arg->a = a;
        arg->b = b;
        arg->c = c;
        arg->len = len;
        arg->core = num_thread;

        res = pthread_create(a_thread + i, NULL, matrixMultiplicationHWThreadKernel, (void*)arg);
        if (res) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_thread; i++) {
        res = pthread_join(a_thread[i], &thread_result);
        if (res) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }
}
