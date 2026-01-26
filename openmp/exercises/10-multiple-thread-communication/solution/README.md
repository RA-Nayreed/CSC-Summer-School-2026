<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

*Note for instructors: use `run*.sh` scripts to generate all the output files.*

## Task: MPI point-to-point communication between OpenMP threads

1. Output with 3 MPI ranks:

       Rank 2 thread 0 received 0
       Rank 1 thread 0 received 0

2. See `multiple-p2p-thread-tags`. Output is correct:

       Rank 2 thread 1 received 1
       Rank 2 thread 2 received 2
       Rank 2 thread 0 received 0
       Rank 2 thread 3 received 3
       Rank 1 thread 2 received 2
       Rank 1 thread 1 received 1
       Rank 1 thread 3 received 3
       Rank 1 thread 0 received 0

   If tag is not specific to thread, then the output is random:

       Rank 2 thread 0 received 3
       Rank 2 thread 3 received 1
       Rank 2 thread 2 received 0
       Rank 2 thread 1 received 2
       Rank 1 thread 1 received 3
       Rank 1 thread 0 received 1
       Rank 1 thread 3 received 0
       Rank 1 thread 2 received 2

3. See `multiple-p2p-thread-comms`. Output correct.

   Note that the different communicators can use the same tag values safely.

## Task: MPI collective communication between OpenMP threads

1. Output like above.

2. See `multiple-coll-thread-comms`. Output correct.

3. See `multiple-coll-single-comm`. Output correct.

   Note that this implementation enforces that the MPI broadcasts are called in the same
   order across all threads. If this is not done, then the results are random.

   Note that this is not a good pattern for performance or code clarity.
   Using the thread-specific communicators is much better idea.
