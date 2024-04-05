#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define NUM_STEPS 1000000

int main(int argc, char* argv[]) {
    int rank, size;
    double step, x, sum = 0.0, pi;
    double* local_pi;
    struct timespec start, end;
    u_int64_t diff;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size <= 0) {
        printf("Error: Need at least 1 process\n");
        MPI_Finalize();
        return 1;
    }

    step = 1.0 / (double)NUM_STEPS;

    // Calculate starting point for each process (avoid overshoot)
    int local_start = rank * (NUM_STEPS / size);
    int local_end = (rank == size - 1) ? NUM_STEPS : (rank + 1) * (NUM_STEPS / size);

    // Allocate memory for local PI estimate
    local_pi = (double*)malloc(sizeof(double));

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = local_start; i < local_end; i++) {
        x = 0.5 * step + i * step;
        sum += 4.0 / (1.0 + x * x);
    }

    *local_pi = step * sum; // Local estimate

    clock
