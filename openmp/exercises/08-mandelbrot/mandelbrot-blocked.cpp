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
#define BLOCK_SIZE 4

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

    for (int j = 0; j < height; j += BLOCK_SIZE) {
        for (int i = 0; i < width; i += BLOCK_SIZE) {
            compute_block(iter_counts.data(), width,
                          i, j, i + BLOCK_SIZE, j + BLOCK_SIZE,
                          xmin, ymin, dx, dy);
        }
    }

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

