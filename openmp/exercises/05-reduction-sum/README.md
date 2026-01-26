<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Parallel sum

In this exercise we practise OpenMP reduction construct.

The provided example code attempts to calculate in parallel the sum

$$
\sum_{i = 0}^{N} \sin(i)
$$

However, there is a race condition in the code as executing the code
multiple times results in different outputs.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.


## Task: Fix the race condition

1. Study, compile, and run the provided code.
   Run the code with different numbers of threads to explore the race condition.

2. Use the `reduction` clause to fix the race condition. Should the `reduction` clause be added to `parallel` or `for`? What's the difference?

3. Alternatively, use the `atomic` (or `critical`) clauses to fix the race condition.
   Compare the execution time to the `reduction` version. What is causing the difference?
   Is it possible to reorganize the code to gain better performance?
