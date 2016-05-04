#include <stdio.h>
#include <pthread.h>

#define DIM 5

int A[DIM * DIM];
int B[DIM * DIM];
int C[DIM * DIM];

void *MatrixMultiplyRow(void *arg);
void InitializeMatrix(int *X);
void MultiplyMatrices();
void MultiplyMatricesParallel();
void PrintMatrix(int *X);

void InitializeMatrix(int *X)
{
	int i, j;
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			X[i * DIM + j] = random() % 10;
		}
	}
}

void PrintMatrix(int *X)
{
	int i, j;
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			printf("%3d ", X[i * DIM + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void MultiplyMatrices()
{
	int i, j, k;
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			int sum = 0;
			for (k = 0; k < DIM; k++)
			{
				sum += A[i * DIM + k] * B[k * DIM + j];
			}
			C[i * DIM + j] = sum;
		}
	}
}

void MultiplyMatricesParallel()
{
	int i;
	int indexes[DIM];
	for(i=0; i<DIM; i++)
	{
		indexes[i] = i;
	}

	pthread_t p[DIM];
	for (i = 0; i < DIM; i++)
	{
		pthread_create(&p[i], NULL, &MatrixMultiplyRow, &indexes[i]);
	}

	for (i = 0; i < DIM; i++)
	{
		pthread_join(p[i], NULL);
	}
}

void *MatrixMultiplyRow(void *arg)
{
	int i, j, k;
	i = *((int*)arg);
	for (j = 0; j < DIM; j++)
	{
		int sum = 0;
		for (k = 0; k < DIM; k++)
		{
			sum += A[i * DIM + k] * B[k * DIM + j];
		}
		C[i * DIM + j] = sum;
	}
}

int main()
{
	InitializeMatrix(A);
	InitializeMatrix(B);
	MultiplyMatricesParallel();
	PrintMatrix(A);
	PrintMatrix(B);
	PrintMatrix(C);
	return 0;
}