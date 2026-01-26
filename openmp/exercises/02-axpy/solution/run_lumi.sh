#!/bin/bash
#SBATCH --job-name=test
#SBATCH --partition=debug
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=128
#SBATCH --mem=0
#SBATCH --time=00:10:00

set -xeuo pipefail

CC="CC -O3 -Wall"
FT="ftn -O3"

$CC ../axpy.cpp -o axpy.x
$FT ../axpy.F90 -o axpy-f.x
srun --cpus-per-task=1 -o axpy-serial.out ./axpy.x
srun --cpus-per-task=1 -o axpy-f-serial.out ./axpy-f.x
diff axpy-serial.out axpy-f-serial.out

$CC -fopenmp axpy.cpp -o axpy.x
$FT -fopenmp axpy.F90 -o axpy-f.x
srun --cpus-per-task=2 -o axpy-t2.out ./axpy.x
srun --cpus-per-task=2 -o axpy-f-t2.out ./axpy-f.x
diff axpy-t2.out axpy-f-t2.out

$CC -fopenmp axpy-timed.cpp -o axpy.x
$FT -fopenmp axpy-timed.F90 -o axpy-f.x
for c in 1 2 4 8 16; do
    srun --cpus-per-task=$c -o axpy-timed-t$c.out ./axpy.x 102400000
    grep "took" axpy-timed-t$c.out
    srun --cpus-per-task=$c -o axpy-timed-f-t$c.out ./axpy-f.x 102400000
    grep "took" axpy-timed-f-t$c.out
    srun --cpus-per-task=$c -o axpy-timed-t$c.out ./axpy.x 102400000
    grep "took" axpy-timed-t$c.out
    srun --cpus-per-task=$c -o axpy-timed-f-t$c.out ./axpy-f.x 102400000
    grep "took" axpy-timed-f-t$c.out
done

