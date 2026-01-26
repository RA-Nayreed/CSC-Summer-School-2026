<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

*Note for instructors: use `run*.sh` scripts to generate all the output files.*

## Task: Parallelize with OpenMP threads

1. Output from the serial code in `axpy-serial.out`.

2. See `axpy.{cpp,F90}`. Output with two threads in `axpy-t2.out`.

3. See `axpy-timed.{cpp,F90}`. Output with various thread counts in `axpy-timed-t*.out`.

   Calculating axpy of a large array of 102400000 elements takes on LUMI takes about 60 ms with 1 to 16 threads.

   This seems rather appalling performance improvement for such a code with independent loop iterations.
   The reason is two-fold: 1) the axpy operation is bound by the memory bandwidth and 2) the current
   code is not managing memory allocation well for high performance.

   We'll resolve this issue later when we discuss non-uniform memory access on modern processors
   (see the follow-up exercise).
