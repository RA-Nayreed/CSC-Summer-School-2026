<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

## Task: Single-thread execution

1. See `execution-control-masked.cpp`. Example output:

       Main thread: initial var = 42
       Thread    1:   final var = 1
       Thread    2:   final var = 2
       Thread    0: initial var = 42
       Thread    0:   final var = 0
       Thread    3:   final var = 3
       Main thread:   final var = 42

   There is no implicit barrier after `masked` (or `master`).

2. See `execution-control-masked-last.cpp`.
   Alternative to

       #pragma omp masked filter(id)

   would be

       if (omp_get_thread_num() == id)

3. See `execution-control-single.cpp`. Example output:

       Main thread: initial var = 42
       Thread    1: initial var = 42
       Thread    2:   final var = 2
       Thread    1:   final var = 1
       Thread    0:   final var = 0
       Thread    3:   final var = 3
       Main thread:   final var = 42

   There is an implicit barrier after `single`.

4. See `execution-control-single-nowait.cpp`. Example output:

       Main thread: initial var = 42
       Thread    2:   final var = 2
       Thread    3: initial var = 42
       Thread    3:   final var = 3
       Thread    0:   final var = 0
       Thread    1:   final var = 1
       Main thread:   final var = 42

   The `nowait` clause removes the implicit barrier of `single`.


## Task: Synchronization

1. See `execution-control-barrier.cpp`. Example output:

       Main thread: initial var = 42
       Thread    3: initial var = 42
       Thread    2: initial var = 42
       Thread    0: initial var = 42
       Thread    1: initial var = 42
       Thread    1:   final var = 1
       Thread    3:   final var = 3
       Thread    0:   final var = 0
       Thread    2:   final var = 2
       Main thread:   final var = 42


## Task: Critical regions

1. See `execution-control-critical.cpp`. Example output:

       Main thread: initial var = 42
       Thread    1: initial var = 42
       Thread    1:   final var = 1
       Thread    2: initial var = 42
       Thread    2:   final var = 2
       Thread    3: initial var = 42
       Thread    3:   final var = 3
       Thread    0: initial var = 42
       Thread    0:   final var = 0
       Main thread:   final var = 42

   This code is essentially serial as the whole parallel region consists
   of a critical region.

   Alternative code for this particular case is shown in
   `execution-control-critical-alt.cpp`.

2. The `atomic` is limited to updating a single value, and
   it's not applicable here.
