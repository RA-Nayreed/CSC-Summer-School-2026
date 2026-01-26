<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Execution control

In this exercise we practise OpenMP execution control constructs.

The provided code is the "`firstprivate`" variant of the previous exercise's solution code.
See the previous exercise for a description of the code.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.


## Task: Single-thread execution

1. Use the `masked` or `master` clause so that only thread 0 prints the initial value within the parallel region.
   Run the program multiple times. Do other threads wait for the masked print to finish?

   Note: `masked` is defined in OpenMP 5.1, so using it requires a supporting compiler.
   Be sure to compile with `-Wall` (or `-Wunknown-pragmas`) to see if the compiler supports it.
   For example, GCC 12 or newer is required to use `masked`.

2. Use the `masked` clause so that only the last thread prints the initial value.
   How could you do the equivalent behavior without `masked` clause?

3. Use the `single` clause so that only a single (but any) thread prints the initial value.
   Run the program multiple times. Do other threads wait for the single print to finish?

4. Add `nowait` clause to the `single` clause.
   Do the other threads now wait for the single print to finish?


## Task: Synchronization

1. Start from the initial code in this task.

   Use the `barrier` construct so that all threads print their initial values
   before any final value is printed.


## Task: Critical regions

1. Start from the initial code in this task.

   Depending on timing, the initial and final print of each thread do not always
   come out consecutively, but some other threads might have printed in between.

   Use the `critical` construct to ensure that all threads print both their
   initial and final value so that no other thread can make a print in between.

   Consider the resulting code. Is the code efficient? Would there be a better
   way to achieve the same behaviour?

2. Could you use the `atomic` construct to achieve the same behavior as `critical`?
