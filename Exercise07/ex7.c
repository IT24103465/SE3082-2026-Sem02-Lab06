#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NPOINTS 2000
#define MAXITER 2000

int main()
{
    const double xmin = -2.0;
    const double xmax = 0.5;
    const double ymin = 0.0;
    const double ymax = 1.125;

    long long inside = 0;
    double tstart = omp_get_wtime();

    #pragma omp parallel for schedule(static) reduction(+:inside)
    for (int i = 0; i < NPOINTS; i++) {
        for (int j = 0; j < NPOINTS; j++) {
            double cx =
                xmin + (xmax - xmin) * i / (NPOINTS - 1.0);

            double cy =
                ymin + (ymax - ymin) * j / (NPOINTS - 1.0);

            double zx = cx;
            double zy = cy;
            int iteration = 0;

            while (iteration < MAXITER) {
                double zxNew = zx * zx - zy * zy + cx;
                double zyNew = 2.0 * zx * zy + cy;

                zx = zxNew;
                zy = zyNew;

                if (zx * zx + zy * zy > 4.0)
                    break;

                iteration++;
            }

            if (iteration == MAXITER)
                inside++;
        }
    }

    double tstop = omp_get_wtime();

    double boxArea =
        (xmax - xmin) * (ymax - ymin);

    double mandelbrotArea =
        2.0 * boxArea * inside /
        ((double)NPOINTS * NPOINTS);

    printf("Points inside = %lld\n", inside);
    printf("Estimated Mandelbrot area = %.8f\n",
           mandelbrotArea);
    printf("Threads used = %d\n", omp_get_max_threads());
    printf("Time = %f seconds\n", tstop - tstart);

    return 0;
}