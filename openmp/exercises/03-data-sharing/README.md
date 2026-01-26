<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Data sharing in parallel regions

In this exercise we practise OpenMP data-sharing clauses.

The provided code modifies the variable `var` in parallel region
and prints its value in different contexts:
- With the main thread before the parallel region
- With each thread in the parallel region, before modifying `var`
- With each thread in the parallel region, after modifying `var`
- With the main thread after the parallel region

Possible outcome of the program run with four threads is:

    Main thread: initial var = 42
    Thread    3: initial var = 42
    Thread    3:   final var = 3
    Thread    1: initial var = 3
    Thread    1:   final var = 1
    Thread    2: initial var = 42
    Thread    0: initial var = 42
    Thread    0:   final var = 0
    Thread    2:   final var = 2
    Main thread:   final var = 2

There is a race condition in the program as the output varies between different runs.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.


## Task: Data-sharing clauses

1. Study, compile, and run the provided code with different numbers of threads.

2. Declare the variable `var` as `private` in the parallel region.
   What is the output now?

3. Declare the variable `var` as `firstprivate` in the parallel region.
   What is the output now?

4. Declare the variable `var` as `shared` in the parallel region.
   What is the output now?

5. Add `default(none)` clause for the parallel directive. What happens if no data-sharing
   clause is defined for `var`?
