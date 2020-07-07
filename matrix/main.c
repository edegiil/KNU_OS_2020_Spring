#include "matrix.h"

int main(int argc, char** argv)
{
    int **a;
    int **b;
    int **c;
    int len, num_thread;
    double start, stop;

    if (argc == 2) {
        len = atoi(argv[1]);
        num_thread = 1;
    } else if (argc == 3) {
        len = atoi(argv[1]);
        num_thread = atoi(argv[2]);
    } else {
        len = 10;
        num_thread = 1;
    }

    initialize(&a, &b, &c, len);

    // printf("Single Thread\n");
    // start = wtime();
    // matrixMultiplication (a, b, c, len);
    // stop = wtime();
    // printMatrix (a, b, c, len);

    // printf("processing time: %fs\n", stop - start);

    printf("Multi Thread with %d threads\n", len);
    start = wtime();
    matrixMultiplicationThread (a, b, c, len);
    stop = wtime();
    printMatrix (a, b, c, len);
    
    printf("processing time: %fs\n", stop - start);

    printf("Multi Thread with %d threads\n", num_thread);
    start = wtime();
    matrixMultiplicationHWThread (a, b, c, len, num_thread);
    stop = wtime();

    printMatrix (a, b, c, len);
    
    printf("processing time: %fs\n", stop - start);

    return 0;
}


double wtime()
{
    static int sec = -1;
    struct timeval tv;
    gettimeofday(&tv, NULL);

    if (sec < 0) sec = tv.tv_sec;

    return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
}
