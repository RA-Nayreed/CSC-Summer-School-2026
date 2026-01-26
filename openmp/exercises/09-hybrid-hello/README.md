<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Hybrid hello world

In this exercise we practise the compiling and running an MPI+OpenMP code.

A minimal OpenMP example code is provided: The program prints a hello message for each threads.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.


## Task: Add MPI parallelization

1. Add the required MPI constructs in order to extend the program to print out the MPI rank in addition to the OpenMP thread id.
2. Print out the provided thread support and compare it to the values of different thread support levels.
