#!/bin/bash
#SBATCH --job-name=test
#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=3
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

set -xeuo pipefail

rm -f ../*.x *.x

CC="CC -O3 -Wall"
FT="ftn -O3"

for f in ../*.cpp; do
    $CC -fopenmp "$f" -o "${f%.cpp}.x"
done

export OMP_DISPLAY_AFFINITY=true
export OMP_AFFINITY_FORMAT="Process %P level %L thread %0.4n/%0.4N on node %H core %A"
srun ../hello.x

export OMP_NUM_THREADS=2
for b in spread close master; do
    for p in threads cores sockets; do
        export OMP_PLACES=$p
        export OMP_PROC_BIND=$b
        srun ../hello.x
    done
done

