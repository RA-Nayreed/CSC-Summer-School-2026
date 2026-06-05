<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

## Heat equation solver in parallel with OpenMP

In this exercise, your task is to parallelize the two dimensional heat equation using OpenMP.
See previous sections for a general description of the heat equation solver.

Starting point is a working serial code, which you should parallelize
by inserting appropriate OpenMP directives and routines.

1. Determine and print the number of threads in the main routine ([cpp/main.cpp](cpp/main.cpp) or [fortran/main.F90](fortran/main.F90))
2. Parallelize the generation of initial temperature in the routine  `generate_field()` (in [fortran/setup.F90](fortran/setup.F90)) or in the `generate()` method (in [cpp/heat.cpp](cpp/heat.cpp)
3. Parallelize the main computational routine
   `evolve()` in [cpp/core.cpp](cpp/core.cpp) or [fortran/core.F90](fortran/core.F90).

To build the code, please use the provided `Makefile` (by typing `make`).
