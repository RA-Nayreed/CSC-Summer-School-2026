// SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cmath>
#include <random>
#include <omp.h>

int main(int argc, char* argv[])
{
    // Number of Monte Carlo samples
    int N = 10000000;
    if (argc > 1) {
        N = std::stoi(argv[1]);
    }
    printf("Samples: %d\n", N);

    // Seed (random by default)
    std::random_device rd;
    unsigned int seed = rd();
    if (argc > 2) {
        seed = std::stoi(argv[2]);
    }
    printf("Seed: %d + thread number\n", static_cast<int>(seed));

    // Start timing
    double t0 = omp_get_wtime();

    double total_distance = 0.0;

    #pragma omp parallel reduction(+:total_distance)
    {
        // Create a random number generator
        std::mt19937_64 rng(seed + omp_get_thread_num());
        std::uniform_real_distribution<double> dis(0.0, 1.0);

        // Print a few random values for debugging
        printf("Thread %3d: A few random values: %.4f %.4f %.4f\n",
               omp_get_thread_num(), dis(rng), dis(rng), dis(rng));

        // Draw N random lines and calculate total distance
        #pragma omp for
        for (int i = 0; i < N; ++i) {
            // Get two random points
            double x1 = dis(rng);
            double y1 = dis(rng);
            double x2 = dis(rng);
            double y2 = dis(rng);

            // Calculate distance between the points
            double dx = x1 - x2;
            double dy = y1 - y2;
            double distance = sqrt(dx * dx + dy * dy);

            // Sum up distances
            total_distance += distance;
        }
    }

    // Calculate average distance
    double average_distance = total_distance / static_cast<double>(N);

    // End timing
    double t1 = omp_get_wtime();

    printf("Average distance: %.6f\n", average_distance);
    printf("Calculation took %.3f milliseconds\n", (t1 - t0) * 1e3);

    return 0;
}
