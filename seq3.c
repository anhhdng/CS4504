#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define FactorIntToDouble 1.1

void matrixMultiSequential(double **firstMatrix, double **secondMatrix, double **matrixMultiResult, int N)
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            double resultValue = 0;
            for (int transNumber = 0; transNumber < N; transNumber++)
            {
                resultValue += firstMatrix[row][transNumber] * secondMatrix[transNumber][col];
            }
            matrixMultiResult[row][col] = resultValue;
        }
    }
}

void matrixInit(double **firstMatrix, double **secondMatrix, int N)
{
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
    for (int N = 512; N <= 2048; N *= 2)
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
        matrixMultiSequential(firstMatrix, secondMatrix, matrixMultiResult, N);
        clock_t t2 = clock();

        printf("Sequential time for N=%d: %.3f seconds\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

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
    }

    return 0;
}
