// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main(int argc, char *argv[])
{
    int rank = 0;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from thread %d in process %d!\n", tid, rank);
    }

    return 0;
}
