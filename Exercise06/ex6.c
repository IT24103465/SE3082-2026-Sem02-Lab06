#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
#define STRIP_SIZE 1024

int main()
{
    double *A = malloc(N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *C = malloc(N * sizeof(double));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        A[i] = i * 0.5;
        B[i] = 2.0;
        C[i] = 0.0;
    }

    double tstart = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int start = 0; start < N;
         start += STRIP_SIZE) {

        int end = start + STRIP_SIZE;

        if (end > N)
            end = N;

        #pragma omp simd
        for (int i = start; i < end; i++) {
            C[i] = A[i] * B[i];
        }
    }

    double tstop = omp_get_wtime();

    printf("C[10] = %f\n", C[10]);
    printf("C[999999] = %f\n", C[999999]);
    printf("Time = %f seconds\n", tstop - tstart);

    free(A);
    free(B);
    free(C);

    return 0;
}