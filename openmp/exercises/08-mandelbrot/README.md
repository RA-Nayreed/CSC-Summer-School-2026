<!--
SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# Exercise: Mandelbrot set

In this exercise we practise computing the Mandelbrot set using OpenMP.

The Mandelbrot set is a set of complex numbers defined by the iterative relation

$$
z_{n+1} = z_n^2 + c
$$

where $z_0 = 0$ and $c$ is a complex number corresponding to a point in the complex plane.
A point $c$ belongs to the Mandelbrot set if the sequence does not diverge, i.e., $|z_n| \le 2$ for all $n$.

In practice, the iteration is performed up to a fixed maximum number of iterations.
For each point, we record the **iteration count**, i.e., the number of iterations needed
for $|z_n|$ to exceed 2. Points that do not diverge within the maximum number of iterations
are considered part of the Mandelbrot set.

The iteration count is also used for visualization: each pixel is colored based on how quickly it diverges,
producing the characteristic fractal image.

The provided serial example codes map each pixel to a point in the complex plane,
computes the iteration count of the pixel, and stores it in an output array.
The iteration counts are stored in a file `image.bin` that can be visualized with the Python script `plot.py`.

Three code variants of increasing complexity are provided:
1. `mandelbrot-direct`: Computes the Mandelbrot set pixel by pixel, following [the algorithm exemplified in Wikipedia](https://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings).
2. `mandelbrot-blocked`: Divides the image into fixed-size blocks and processes the pixels block by block, enabling more structured computation.
3. `mandelbrot-adaptive`: Uses an adaptive algorithm that recursively subdivides blocks into smaller and smaller blocks.
   The subdivision is performed only when necessary, based on an estimate whether the block contains varying values,
   which is determined by comparing the iteration counts at the block's corner points and its center.

The [solution directory](solution/) contains a model solution and discussion on the exercises below.


## Task: Parallelize with OpenMP threads

1. Study, compile, and run the provided serial codes. You can provide the image size as a command line argument, e.g.,
   `./mandelbrot.x 12` computes the image of $2^{12}$ pixels in width and height.

   Please start from the `mandelbrot-direct` code, then compare how `mandelbrot-blocked` differs from it,
   and finally compare how `mandelbrot-adaptive` modifies the algorithm.

   What size images you can compute with the different serial codes in around ten seconds?

   Use LUMI web interface to visualize the beautiful fractal images.

2. Parallelize the different variants using OpenMP. Let's start in a reverse order.

   Use OpenMP tasks to parallelize `mandelbrot-adaptive`. Define a parallel region
   and use tasks to execute the recursive subdivision in parallel.

   Do you see benefit in the execution time?

3. Use OpenMP to parallelize the block loop(s) in `mandelbrot-blocked`.

   Compare `static` and `dynamic` loop scheduling. Which one gives better performance? Why?

4. Use OpenMP to parallelize the pixel loop(s) in `mandelbrot-direct`.

   Consider loop scheduling again. Do you get better or worse performance than with `mandelbrot-blocked`? Why?

