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

#########################################################
# MPI only
#########################################################
for f in ../*.cpp; do
    $CC "$f" -o "${f%.cpp}.x"
done

for f in ../*.x; do
    srun -o $(basename "${f%.x}-ref.out") "$f"
done

#########################################################
# MPI+OpenMP
#########################################################
sed 's/int tag = tid;/int tag = 123;/g' multiple-p2p-thread-tags.cpp > multiple-p2p-random.cpp
for f in *.cpp; do
    $CC -fopenmp "$f" -o "${f%.cpp}.x"
done

for f in *.x; do
    srun -o "${f%.x}.out" "$f"
done
