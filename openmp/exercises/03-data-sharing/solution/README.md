<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

## Task: Data-sharing clauses

1. The output varies from run to run, for example:

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

   or:

       Main thread: initial var = 42
       Thread    0: initial var = 42
       Thread    0:   final var = 0
       Thread    2: initial var = 42
       Thread    2:   final var = 2
       Thread    3: initial var = 42
       Thread    3:   final var = 3
       Thread    1: initial var = 42
       Thread    1:   final var = 1
       Main thread:   final var = 1

2. See `data-sharing-private.cpp`. The output is now:

       Main thread: initial var = 42
       Thread    2: initial var = 0
       Thread    2:   final var = 2
       Thread    3: initial var = 0
       Thread    3:   final var = 3
       Thread    0: initial var = 32655
       Thread    0:   final var = 0
       Thread    1: initial var = 0
       Thread    1:   final var = 1
       Main thread:   final var = 42

    Note that the initial value the threads print is unitialized
    and hence random.

    Note also that the value of `var` in the main thread
    is unchanged.


3. See `data-sharing-firstprivate.cpp`. The output is now:

       Main thread: initial var = 42
       Thread    0: initial var = 42
       Thread    0:   final var = 0
       Thread    3: initial var = 42
       Thread    3:   final var = 3
       Thread    2: initial var = 42
       Thread    2:   final var = 2
       Thread    1: initial var = 42
       Thread    1:   final var = 1
       Main thread:   final var = 42

    Note also that the value of `var` in the main thread
    is unchanged.

4. See `data-sharing-shared.cpp`. The behavior is the same
   as with the initial code. The variable `var` is shared by default.

5. See `data-sharing-default-none.cpp`. The compilation fails with an error:

       data-sharing-default-none.cpp: In function ‘int main()’:
       data-sharing-default-none.cpp:15:15: error: ‘var’ not specified in enclosing ‘parallel’
          15 |         printf("Thread  %3d: initial var = %d\n", omp_get_thread_num(), var);
             |         ~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       data-sharing-default-none.cpp:13:13: note: enclosing ‘parallel’
          13 |     #pragma omp parallel default(none)
             |             ^~~

   The `default(none)` dictates that there is no default data-sharing behaviour,
   but it needs to be declared explictly for each variable used in the parallel region.
