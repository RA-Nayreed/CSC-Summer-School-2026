<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

## Task: Fix the race condition

1. Example outputs with four threads:

       Array size: 100000
       Sum: 0.800947
       Calculation took 0.383 milliseconds

       Array size: 100000
       Sum: 0.800947
       Calculation took 0.336 milliseconds

       Array size: 100000
       Sum: 0.491960
       Calculation took 0.343 milliseconds

       Array size: 100000
       Sum: 0.800947
       Calculation took 0.372 milliseconds

2. See `sum-reduction.cpp`. In this example, the `reduction` clause can be added to either to the `parallel` or
   the `for`/`do` construct. The difference is the scope of reduction, which would matter if the parallel
   region would be larger.
   The output is now consitently the same with different numbers of threads (four threads in this example):

       Array size: 100000
       Sum: 1.812028
       Calculation took 0.339 milliseconds

3. See `sum-atomic-naive.cpp` for one possible code. Output with four threads:

       Array size: 100000
       Sum: 1.812028
       Calculation took 6.045 milliseconds

   The results are correct, but the code is very slow. The `atomic` clause in the loop
   body causes that the loop iterations are serialized.

   See `sum-atomic-better.cpp` for a better implementation. Output with four threads:

       Array size: 100000
       Sum: 1.812028
       Calculation took 0.374 milliseconds

   This implementation calculates thread-local partial sums in parallel
   and only the global summation of the partial sums is done with `atomic` clause.

   Note that in this code the final reduction is still serialized.
   The `reduction` clause can do the final reduction with a better algorithm (e.g. a tree algorithm),
   which can result in improved performance especially with large thread counts.

   That's the core benefit of the `reduction` clause: simpler and more efficient code
   for reduction patterns.
