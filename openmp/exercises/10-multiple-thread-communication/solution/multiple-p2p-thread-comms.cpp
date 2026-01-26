// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <vector>
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

    // Create thread-specific communicators
    std::vector<MPI_Comm> mpi_comm_thread(omp_get_max_threads());
    for (auto& comm : mpi_comm_thread) {
        MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int msg = -1;
        int tag = 123;

        if (rank == 0) {
            msg = tid;
            for (int i = 1; i < ntasks; i++) {
                MPI_Send(&msg, 1, MPI_INT, i, tag, mpi_comm_thread[tid]);
            }
        } else {
            MPI_Recv(&msg, 1, MPI_INT, 0, tag, mpi_comm_thread[tid], MPI_STATUS_IGNORE);
            printf("Rank %d thread %d received %d\n", rank, tid, msg);
        }
    }

    // Free communicators
    for (auto& comm : mpi_comm_thread) {
        MPI_Comm_free(&comm);
    }

    MPI_Finalize();
    return 0;
}
