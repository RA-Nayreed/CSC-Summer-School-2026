// SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#else
#include "fake_omp.h"
#endif

#define MAX_ITER 255
#define MIN_BLOCK_SIZE 4

using iter_t = uint8_t;
using index_t = int64_t;


// Compute a single point
iter_t compute_point(double x, double y) {
    double zr = 0.0;
    double zi = 0.0;
    iter_t iter = 0;

    while (zr * zr + zi * zi <= 4.0 && iter < MAX_ITER) {
        double zr_new = zr * zr - zi * zi + x;
        zi = 2.0 * zr * zi + y;
        zr = zr_new;
        iter++;
    }

    return iter;
}


// Compute a block of pixels
void compute_block(iter_t* iter_counts, index_t width,
                   index_t i0, index_t j0, index_t i1, index_t j1,
                   double xmin, double ymin,
                   double dx, double dy) {
    for (index_t j = j0; j < j1; ++j) {
        for (index_t i = i0; i < i1; ++i) {
            double x = xmin + i * dx;
            double y = ymin + j * dy;
            iter_counts[j * width + i] = compute_point(x, y);
        }
    }
}


// Fill a block of pixels with a constant value
void fill_block(iter_t* iter_counts, index_t width,
                index_t i0, index_t j0, index_t i1, index_t j1,
                iter_t value) {
    for (index_t j = j0; j < j1; ++j) {
        for (index_t i = i0; i < i1; ++i) {
            iter_counts[j * width + i] = value;
        }
    }
}


// Compute or subdivide a block using recursive adaptive refinement
void compute_adaptive(iter_t* iter_counts, index_t width,
                      index_t i0, index_t j0, index_t i1, index_t j1,
                      double xmin, double ymin, double dx, double dy) {

    // Compute small block directly
    index_t w = i1 - i0;
    index_t h = j1 - j0;
    if (w <= MIN_BLOCK_SIZE && h <= MIN_BLOCK_SIZE) {
        compute_block(iter_counts, width,
                      i0, j0, i1, j1,
                      xmin, ymin, dx, dy);
        return;
    }

    // Sample corners and center
    double x0 = xmin + i0 * dx;
    double x1 = xmin + (i1 - 1) * dx;
    double xm = 0.5 * (x0 + x1);

    double y0 = ymin + j0 * dy;
    double y1 = ymin + (j1 - 1) * dy;
    double ym = 0.5 * (y0 + y1);

    iter_t c00 = compute_point(x0, y0);
    iter_t c10 = compute_point(x1, y0);
    iter_t c01 = compute_point(x0, y1);
    iter_t c11 = compute_point(x1, y1);
    iter_t cmm = compute_point(xm, ym);

    // Fill the block uniformly if all the samples are the same
    if (c00 == c10 && c00 == c01 && c00 == c11 && c00 == cmm) {
        fill_block(iter_counts, width, i0, j0, i1, j1, c00);
        return;
    }

    // Subdivide the block otherwise
    index_t im = (i0 + i1) / 2;
    index_t jm = (j0 + j1) / 2;
    compute_adaptive(iter_counts, width,
                     i0, j0, im, jm,
                     xmin, ymin, dx, dy);

    compute_adaptive(iter_counts, width,
                     im, j0, i1, jm,
                     xmin, ymin, dx, dy);

    compute_adaptive(iter_counts, width,
                     i0, jm, im, j1,
                     xmin, ymin, dx, dy);

    compute_adaptive(iter_counts, width,
                     im, jm, i1, j1,
                     xmin, ymin, dx, dy);
}


int main(int argc, char* argv[]) {
    // Size of image as power of two
    unsigned p = 10;
    if (argc > 1) {
        p = std::stoi(argv[1]);
    }
    const index_t width = 1 << p;
    const index_t height = width;
    printf("Image size: %ldx%ld\n", width, height);

    double t0, t1;

    // Define viewing window in complex plane
    double xmin = -1.7, xmax = 0.7;
    double ymin = -1.2, ymax = 1.2;

    // Pixel spacing
    double dx = (xmax - xmin) / (width - 1);
    double dy = (ymax - ymin) / (height - 1);

    // Allocate data array
    std::vector<iter_t> iter_counts(width * height);

    // Start timing
    t0 = omp_get_wtime();

    compute_adaptive(iter_counts.data(), width,
                     0, 0, width, height,
                     xmin, ymin, dx, dy);

    // End timing
    t1 = omp_get_wtime();

    printf("Calculation took %.3f milliseconds\n", (t1 - t0) * 1e3);

    // Start timing
    t0 = omp_get_wtime();

    // Binary output
    FILE* f = fopen("image.bin", "wb");
    fwrite(&width, sizeof(index_t), 1, f);
    fwrite(&height, sizeof(index_t), 1, f);
    fwrite(iter_counts.data(), sizeof(iter_t), width * height, f);
    fclose(f);

    // End timing
    t1 = omp_get_wtime();

    printf("Writing file took %.3f milliseconds\n", (t1 - t0) * 1e3);

    printf("Mandelbrot data saved to image.bin\n");
    return 0;
}

