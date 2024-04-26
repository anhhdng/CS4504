#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 2048
#define BlockSize 64
#define FactorIntToDouble 1.1

double firstMatrix[N][N] = {0.0};
double secondMatrix[N][N] = {0.0};
double matrixMultiResult[N][N] = {0.0};

void matrixMultiBlock()
{
    #pragma omp parallel for collapse(2)
    for (int bi = 0; bi < N; bi += BlockSize)
    {
        for (int bj = 0; bj < N; bj += BlockSize)
        {
            for (int bk = 0; bk < N; bk += BlockSize)
            {
                for (int i = bi; i < bi + BlockSize && i < N; i++)
                {
                    for (int j = bj; j < bj + BlockSize && j < N; j++)
                    {
                        double resultValue = 0;
                        for (int k = bk; k < bk + BlockSize && k < N; k++)
                        {
                            resultValue += firstMatrix[i][k] * secondMatrix[k][j];
                        }
                        matrixMultiResult[i][j] += resultValue;
                    }
                }
            }
        }
    }
}

void matrixInit()
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
    matrixInit();

    clock_t t1 = clock();
    matrixMultiBlock();
    clock_t t2 = clock();
    printf("time: %ld\n", t2 - t1);

    return 0;
}
