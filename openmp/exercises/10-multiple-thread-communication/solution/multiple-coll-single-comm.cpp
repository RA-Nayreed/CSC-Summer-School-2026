// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>


int main(int argc, char *argv[])
{
    int provided, rank, ntasks;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    /* Check that the MPI implementation supports MPI_THREAD_MULTIPLE */
    if (provided < MPI_THREAD_MULTIPLE) {
        printf("MPI does not support MPI_THREAD_MULTIPLE\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int msg = -1;

        if (rank == 0) {
            msg = tid;
        }

        // Call broadcast with threads in order (once per thread; first thread 0, then thread 1, 2, ...)
        #pragma omp for ordered schedule(static, 1)
        for (int i = 0; i < omp_get_num_threads(); i++) {
            #pragma omp ordered
            MPI_Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
 
        if (rank > 0) {
            printf("Rank %d thread %d received %d\n", rank, tid, msg);
        }
    }

    MPI_Finalize();
    return 0;
}
