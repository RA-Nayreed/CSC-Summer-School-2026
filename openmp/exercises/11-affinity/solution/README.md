<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

*Note for instructors: use `run*.sh` scripts to generate all the output files.*

## Tasks

1. Output on LUMI with `--ntasks-per-node=3 --cpus-per-task=4`:

       Process 49127 level 0 thread 0000/0001 on node nid002275 core  8-11
       Process 49127 level 0 thread 0000/0001 on node nid002275 core  8
       Hello from thread 0 in process 2!
       Process 49127 level 1 thread 0002/0004 on node nid002275 core  10
       Hello from thread 2 in process 2!
       Process 49127 level 1 thread 0001/0004 on node nid002275 core  9
       Hello from thread 1 in process 2!
       Process 49127 level 1 thread 0003/0004 on node nid002275 core  11
       Hello from thread 3 in process 2!
       Process 49125 level 0 thread 0000/0001 on node nid002275 core  0-3
       Process 49125 level 0 thread 0000/0001 on node nid002275 core  0
       Hello from thread 0 in process 0!
       Process 49125 level 1 thread 0002/0004 on node nid002275 core  2
       Hello from thread 2 in process 0!
       Process 49125 level 1 thread 0003/0004 on node nid002275 core  3
       Hello from thread 3 in process 0!
       Process 49125 level 1 thread 0001/0004 on node nid002275 core  1
       Hello from thread 1 in process 0!
       Process 49126 level 0 thread 0000/0001 on node nid002275 core  4-7
       Process 49126 level 0 thread 0000/0001 on node nid002275 core  4
       Hello from thread 0 in process 1!
       Process 49126 level 1 thread 0001/0004 on node nid002275 core  5
       Hello from thread 1 in process 1!
       Process 49126 level 1 thread 0002/0004 on node nid002275 core  6
       Hello from thread 2 in process 1!
       Process 49126 level 1 thread 0003/0004 on node nid002275 core  7
       Hello from thread 3 in process 1!

   Threads are bound to cores by default.

2. Output of the example case

       Process 49171 level 0 thread 0000/0001 on node nid002275 core  0
       Hello from thread 0 in process 0!
       Process 49171 level 1 thread 0001/0002 on node nid002275 core  2
       Hello from thread 1 in process 0!
       Process 49173 level 0 thread 0000/0001 on node nid002275 core  8
       Hello from thread 0 in process 2!
       Process 49173 level 1 thread 0001/0002 on node nid002275 core  10
       Hello from thread 1 in process 2!
       Process 49172 level 0 thread 0000/0001 on node nid002275 core  4
       Hello from thread 0 in process 1!
       Process 49172 level 1 thread 0001/0002 on node nid002275 core  6
       Hello from thread 1 in process 1!

   The two threads per process are spread across the available cores.
