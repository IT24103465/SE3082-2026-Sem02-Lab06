#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

long long fib(int n)
{
    long long i, j;

    if (n < 2)
        return n;

    if (n < 20) {
        return fib(n - 1) + fib(n - 2);
    }

    #pragma omp task shared(i) firstprivate(n)
    i = fib(n - 1);

    #pragma omp task shared(j) firstprivate(n)
    j = fib(n - 2);

    #pragma omp taskwait

    return i + j;
}

int main(int argc, char *argv[])
{
    int n = 40;
    long long result;
    double tstart, tstop;

    if (argc > 1)
        n = atoi(argv[1]);

    tstart = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = fib(n);
        }
    }

    tstop = omp_get_wtime();

    printf("Fibonacci(%d) = %lld\n", n, result);
    printf("Time = %f seconds\n", tstop - tstart);

    return 0;
}