<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Discussion

## Task: Parallelize with OpenMP task

1. See `tasks.cpp`. Output with four threads:

       Start with 0
       A: 0->1
       B: 1->3
       D: 1->5
       C: 1->4
       E: 3->8
       F: 4,5->15
       G: 8,15->30
       End with 30
       Execution took 4000.811 milliseconds
