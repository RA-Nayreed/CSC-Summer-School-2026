// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>


int main(int argc, char *argv[])
{
    int rank, ntasks;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    int tid = 0;
    int msg = -1;
    int tag = 123;

    if (rank == 0) {
        msg = tid;
        for (int i = 1; i < ntasks; i++) {
            MPI_Send(&msg, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&msg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d thread %d received %d\n", rank, tid, msg);
    }

    MPI_Finalize();
    return 0;
}
