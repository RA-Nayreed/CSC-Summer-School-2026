<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Multiple thread communication

In this exercise we demonstrate MPI communication between multiple OpenMP threads.

The provided pure MPI program sends a message from the rank 0 to all the other ranks
that then print the received message.
This is implemented using point-to-point operations (`multiple-p2p`) and
collective operations (`multiple-coll`).

Our aim is to convert these codes to hybrid MPI+OpenMP programs that implement the following desired communication pattern:
Each thread of rank 0 send their thread id to the thread of same id in all the other MPI ranks.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.

## Task: MPI point-to-point communication between OpenMP threads

1. Study, compile, and run the provided MPI code `multiple-p2p`.

2. Use thread-specific tags to implement the desired communication pattern.

   Remember to also update the MPI initialization to the thread-aware version with `MPI_THREAD_MULTIPLE` support level,
   and check that the MPI initilization returned the requested support level.

3. Instead of thread-specific tags, use thread-specific communicators to implement the desired communication pattern.

## Task: MPI collective communication between OpenMP threads

1. Study, compile, and run the provided MPI code `multiple-coll`.

2. Use the same thread-specific communicators as in the previous exercises to implement the desired communication pattern.

3. (Bonus) Would it be possible or sensible to implement the desired communication pattern without using thread-specific communicators?
