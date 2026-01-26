// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main(void)
{
    int var = 42;

    printf("Main thread: initial var = %d\n", var);
    #pragma omp parallel firstprivate(var)
    {
        printf("Thread  %3d: initial var = %d\n", omp_get_thread_num(), var);

        #pragma omp barrier

        var = omp_get_thread_num();
        printf("Thread  %3d:   final var = %d\n", omp_get_thread_num(), var);
    }
    printf("Main thread:   final var = %d\n", var);

    return 0;
}
