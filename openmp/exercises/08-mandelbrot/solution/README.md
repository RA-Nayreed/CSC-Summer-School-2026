<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

*Note for instructors: use `run*.sh` scripts to generate all the output files.*

## Task: Parallelize with OpenMP threads

1. Serial runtimes for reference:
     - `./mandelbrot-direct.x 12`: 2974 ms
     - `./mandelbrot-blocked.x 12`: 2974 ms
     - `./mandelbrot-adaptive.x 14`: 2220 ms

2. See `mandelbrot-adaptive`. Runtime: 553 ms (16 threads).

   See `data/` directory for multiple output files, e.g., `grep Calc data/*.out`.

3. See `mandelbrot-blocked`. The code distributes work block by block by using
   `collapse(2) schedule(dynamic)` and the speed-up is good: 242 ms (16 threads).

   Note that this distribution of work would be equivalent to the tasked recursive
   `mandelbrot-adaptive` if the adaptive component would be removed and
   all the 4x4 blocks would be generated recursively and scheduled via tasks.

   If we'd use `schedule(static)`, then the speed-up is much smaller: 507 ms (16 threads).

   The reason is that computational workload for each block is greatly
   different depending on its location on the complex plane.
   Thus, it is beneficial to pay the overhead of the dynamic scheduling to distribute
   the blocks to the threads during the runtime.

4. See `mandelbrot-direct`. The code distributes only the outer loop with `schedule(dynamic)`,
   and the runtime is similar to the blocked code: 210 ms (16 threads).

   If we'd distribute the work over both loops with `collapse(2) schedule(dynamic)`,
   the code would become slower: 696 ms (16 threads).

   The reason is that this slower code has larger scheduling overhead as
   the work is distributed pixel by pixel. The scheduling overhead is decreased
   by distributing larger chunks of work (rows or blocks).

   This could done also by adjusting the chunk size in the schedule clause.
