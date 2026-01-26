#!/bin/bash
#SBATCH --job-name=test
#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

set -xeuo pipefail

CC="CC -O3 -Wall"
FT="ftn -O3"

$CC -fopenmp hello-1.cpp -o hello.x
srun -o hello-1.out ./hello.x

$CC -fopenmp hello-2.cpp -o hello.x
srun -o hello-2.out ./hello.x
