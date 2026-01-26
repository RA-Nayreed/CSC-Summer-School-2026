<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Inspect and control affinity

In this exercise we explore how to query process and thread affinity.

We use an earlier hybrid hello code as an example code.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.

## Tasks

1. Set OpenMP environment variables to print affinity and run the program:

       export OMP_DISPLAY_AFFINITY=true
       export OMP_AFFINITY_FORMAT="Process %P level %L thread %0.4n/%0.4N on node %H core %A"
       srun ./hello.x

2. Use OpenMP environment variables to set the number of threads as well as their binding and placement, for example:

       export OMP_NUM_THREADS=2  # use also smaller values than --cpus-per-task option for slurm
       export OMP_PLACES=cores
       export OMP_PROC_BIND=spread

   Are the affinities expected?

   See the OpenMP documentation for possible values of these environment variables:
   - [`OMP_PLACES`](https://www.openmp.org/spec-html/5.0/openmpse53.html#x292-20600006.5)
   - [`OMP_PROC_BIND`](https://www.openmp.org/spec-html/5.0/openmpse52.html#x291-20580006.4)

