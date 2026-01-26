#!/bin/bash
#SBATCH --job-name=test
#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --time=00:10:00

set -xeuo pipefail

CC="CC -O3 -Wall"
FT="ftn -O3"

$CC -fopenmp ../hello.cpp -o hello.x

srun --cpus-per-task=1 -o hello-cpus1.out ./hello.x
srun --cpus-per-task=4 -o hello-cpus4.out ./hello.x

export OMP_NUM_THREADS=2
srun --cpus-per-task=4 -o hello-omp2.out ./hello.x
unset OMP_NUM_THREADS

srun --cpus-per-task=8 -o hello-cpus8.out ./hello.x


$CC -fopenmp hello-1.cpp -o hello.x
srun --cpus-per-task=4 -o hello-1-cpus4.out ./hello.x

$CC -fopenmp hello-2.cpp -o hello.x
srun --cpus-per-task=4 -o hello-2-cpus4.out ./hello.x

$CC hello-3.cpp -o hello.x
srun --cpus-per-task=4 -o hello-3-no-omp.out ./hello.x
