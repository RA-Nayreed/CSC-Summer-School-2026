// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <string>
#include <omp.h>

int main(int argc, char* argv[])
{
    // Array size
    int n = 100000;
    if (argc > 1) {
        n = std::stoi(argv[1]);
    }
    printf("Array size: %d\n", n);

    // Start timing
    double t0 = omp_get_wtime();

    // Calculate sum
    double sum = 0;
    #pragma omp parallel
    {
        double thread_sum = 0.0;
        #pragma omp for
        for (int i = 0; i < n; i++) {
            thread_sum += std::sin(static_cast<double>(i));
        }

        #pragma omp atomic
        sum += thread_sum;
    }

    // End timing
    double t1 = omp_get_wtime();

    printf("Sum: %f\n", sum);
    printf("Calculation took %.3f milliseconds\n", (t1 - t0) * 1e3);

    return 0;
}
