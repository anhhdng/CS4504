#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 2048
#define BLOCK_SIZE 32
#define FactorIntToDouble 1.1

void matrixMultiBlock(double **firstMatrix, double **secondMatrix, double **matrixMultiResult, int N)
{
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i += BLOCK_SIZE)
    {
        for (int j = 0; j < N; j += BLOCK_SIZE)
        {
            for (int k = 0; k < N; k += BLOCK_SIZE)
            {
                for (int ii = i; ii < i + BLOCK_SIZE; ii++)
                {
                    for (int jj = j; jj < j + BLOCK_SIZE; jj++)
                    {
                        double sum = 0.0;
                        for (int kk = k; kk < k + BLOCK_SIZE; kk++)
                        {
                            sum += firstMatrix[ii][kk] * secondMatrix[kk][jj];
                        }
                        matrixMultiResult[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

void matrixInit(double **firstMatrix, double **secondMatrix, int N)
{
    #pragma omp parallel for collapse(2)
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            srand(row + col);
            firstMatrix[row][col] = (rand() % 10) * FactorIntToDouble;
            secondMatrix[row][col] = (rand() % 10) * FactorIntToDouble;
        }
    }
}

int main()
{
    double **firstMatrix = malloc(N * sizeof(double *));
    double **secondMatrix = malloc(N * sizeof(double *));
    double **matrixMultiResult = malloc(N * sizeof(double *));

    for (int i = 0; i < N; i++)
    {
        firstMatrix[i] = malloc(N * sizeof(double));
        secondMatrix[i] = malloc(N * sizeof(double));
        matrixMultiResult[i] = malloc(N * sizeof(double));
    }

    matrixInit(firstMatrix, secondMatrix, N);

    clock_t t1 = clock();
    matrixMultiBlock(firstMatrix, secondMatrix, matrixMultiResult, N);
    clock_t t2 = clock();

    printf("Block-optimized parallel time for N=%d: %.3f seconds\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

    // Free memory
    for (int i = 0; i < N; i++)
    {
        free(firstMatrix[i]);
        free(secondMatrix[i]);
        free(matrixMultiResult[i]);
    }
    free(firstMatrix);
    free(secondMatrix);
    free(matrixMultiResult);

    return 0;
}
