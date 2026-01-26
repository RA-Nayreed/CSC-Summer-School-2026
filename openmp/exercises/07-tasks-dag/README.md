<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Directed acyclic graph with tasks

In this exercise we practise OpenMP tasking constructs to execute a directed acyclic graph (DAG) of tasks.


The DAG to be executed is shown below:

<!--
dot -Tpng -Gbgcolor=transparent -Ncolor=white -Nfontcolor=white -Ecolor=white dag.dot -o dag-dark.png
dot -Tpng -Gbgcolor=transparent -Ncolor=black -Nfontcolor=black -Ecolor=black dag.dot -o dag-light.png
-->
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="dag-dark.png">
  <source media="(prefers-color-scheme: light)" srcset="dag-light.png">
  <img src="dag-dark.png">
</picture>

A serial code is provided to execute this DAG in order one node a time.
Executing the code creates this output:

    Start with 0
    A: 0->1
    B: 1->3
    C: 1->4
    D: 1->5
    E: 3->8
    F: 4,5->15
    G: 8,15->30
    End with 30
    Execution took 7000.610 milliseconds

The outputs shows inputs and outputs of each node. For example, node/function A takes 0 as an input
and returns 1.

Each function execution takes one second, so the total execution time is 7 seconds.
Let's use OpenMP tasks to execute as many functions in parallel as possible.


The [solution directory](solution/) contains a model solution and discussion on the exercises below.

## Task: Parallelize with OpenMP tasks

1. Create a parallel region with a single thread launching an OpenMP task for each function execution.
   OpenMP runtime will then take care of executing task with the available threads.
   
   Make sure that the data flow between functions remain correct. The end result should be 30
   as in the serial code.
