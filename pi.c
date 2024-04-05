#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define NUM_STEPS 1000000

int main(int argc, char* argv[]) {
    int rank, size;
    double step, x, sum = 0.0, pi;
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

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = local_start; i < local_end; i++) {
        x = 0.5 * step + i * step;
        sum += 4.0 / (1.0 + x * x);
    }

    pi = step * sum; // Local estimate

    // Reduce partial sums from all processes to get the final PI
    MPI_Reduce(&pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC, &end);

    diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

    if (rank == 0) {
        printf("PI is %.20f\n", pi);
        printf("Elapsed time = %llu nanoseconds\n", (long long unsigned int)diff);
    }

    MPI_Finalize();
    return 0;
}
