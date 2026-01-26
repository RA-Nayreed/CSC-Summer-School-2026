<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Lines in a square

In this exercise we practise parallelizing an example Monte Carlo simulation using OpenMP threads.

The goal is to estimate the average Euclidean distance between two random points on the unit square $$[0,1]\times[0,1]$$.

For two points $$(x_1,y_1)$$ and $$(x_2,y_2)$$, the distance is

$$
d = \sqrt{(x_1-x_2)^2 + (y_1-y_2)^2}.
$$

The average distance is estimated by drawing a large number of random point pairs and averaging the distances.

A serial example code is provided. The code:
generates random point pairs,
computes the distance for each pair,
accumulates the sum of distances, and
prints the final average.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.

## Task: Parallelize with OpenMP threads

1.  Study, compile, and run the provided code.

    The number of Monte Carlo samples $$N$$ and the random seed can be given as a command line argument, for example:

        ./lines.x 10000000 0

    This sets $$N$$ to 10000000 and the seed to 0.

2.  Parallelize the Monte Carlo loop using OpenMP by adding suitable OpenMP directives.

    In particular, consider which variables should be private to each thread,
    and how to correctly accumulate the total sum of distances.

    Test that the program produces reasonable results when using different numbers of threads.

    Hint: Update the "A few random values" print to show a few values for each thread.

3.  Ensure the correct random number generation.

    We need to avoid data races in the random number generation and to
    make sure that all the threads do not generate *identical* random numbers.

    The generator objects used are not thread aware, so each thread need to have their
    own generators and seed values.
