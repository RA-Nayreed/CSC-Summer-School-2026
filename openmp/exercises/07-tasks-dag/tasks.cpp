// SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <unistd.h>
#include <omp.h>

// Function declarations
int func_A(int in);
int func_B(int in);
int func_C(int in);
int func_D(int in);
int func_E(int in);
int func_F(int in1, int in2);
int func_G(int in1, int in2);


int main(void)
{
    int s, a, b, c, d, e, f, g;
    s = 0;

    // Start timing
    double t0 = omp_get_wtime();

    printf("Start with %d\n", s);
    a = func_A(s);
    b = func_B(a);
    c = func_C(a);
    d = func_D(a);
    e = func_E(b);
    f = func_F(c, d);
    g = func_G(e, f);
    printf("End with %d\n", g);

    // End timing
    double t1 = omp_get_wtime();

    printf("Execution took %.3f milliseconds\n", (t1 - t0) * 1e3);

    return 0;
}


///////////////////////////////
// Function definitions
//
// No changes needed here
///////////////////////////////


int func_A(int in)
{
    sleep(1);
    int out = in + 1;
    printf("A: %d->%d\n", in, out);
    return out;
}

int func_B(int in)
{
    sleep(1);
    int out = in + 2;
    printf("B: %d->%d\n", in, out);
    return out;
}

int func_C(int in)
{
    sleep(1);
    int out = in + 3;
    printf("C: %d->%d\n", in, out);
    return out;
}

int func_D(int in)
{
    sleep(1);
    int out = in + 4;
    printf("D: %d->%d\n", in, out);
    return out;
}

int func_E(int in)
{
    sleep(1);
    int out = in + 5;
    printf("E: %d->%d\n", in, out);
    return out;
}

int func_F(int in1, int in2)
{
    sleep(1);
    int out = in1 + in2 + 6;
    printf("F: %d,%d->%d\n", in1, in2, out);
    return out;
}

int func_G(int in1, int in2)
{
    sleep(1);
    int out = in1 + in2 + 7;
    printf("G: %d,%d->%d\n", in1, in2, out);
    return out;
}

