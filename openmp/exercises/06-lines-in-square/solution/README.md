<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion


## Task: Parallelize with OpenMP threads

1. Running the code with default arguments uses a random seed:

       Samples: 10000000
       Seed: 1499684558
       Thread   0: A few random values: 0.2505 0.3620 0.9848
       Average distance: 0.521416
       Calculation took 232.703 milliseconds

   The output with a fixed seed (0):

       Samples: 10000000
       Seed: 0
       Thread   0: A few random values: 0.0396 0.9921 0.1598
       Average distance: 0.521372
       Calculation took 232.494 milliseconds


2. See `lines-wrong.cpp`. Possible output with four threads:

       Samples: 10000000
       Seed: 0
       Thread   0: A few random values: 0.7317 0.4409 0.7829
       Thread   3: A few random values: 0.7317 0.4409 0.7829
       Thread   1: A few random values: 0.7317 0.4409 0.7829
       Thread   2: A few random values: 0.3266 0.5847 0.4754
       Average distance: 0.521395
       Calculation took 103.624 milliseconds

   We see that there is an issue with the random number sampling
   as multiple threads are getting the same random values.

3. See `lines-working.cpp`. Output with four threads:

       Samples: 10000000
       Seed: 0 + thread number
       Thread   1: A few random values: 0.4512 0.1364 0.1339
       Thread   0: A few random values: 0.0396 0.9921 0.1598
       Thread   3: A few random values: 0.5902 0.1958 0.5588
       Thread   2: A few random values: 0.7838 0.8502 0.9036
       Average distance: 0.521431
       Calculation took 65.212 milliseconds

   This code uses `omp_get_thread_num()` to set a different
   seed for each thread in order to create a different series
   of random numbers in each thread.

   Note, however, that this causes that the results depend on
   the number of threads. This is not a problem for solving
   the average distance between the points, which requires
   using large enough number of samples to converge the results.
   However, in some scenarios identical results might be desired,
   but implementing such will cost in terms of performance.
