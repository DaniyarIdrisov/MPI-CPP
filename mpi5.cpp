#include<mpi.h>
#include<iostream>

void forScatterv(int a[], int b[], int N, int size) {
    int n = N;
    int localN = n / size;
    a[0] = localN;
    b[0] = 0;
    for (int i = 1; i < size; i++) {
        n -= localN;
        localN = n / (size - i);
        a[i] = localN;
        b[i] = b[i - 1] + a[i - 1];
    }
}

void generateArray(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = rand() / 100;
        b[i] = rand() / 100;
        printf("a[%d] = %d, ", i, a[i]);
        printf("b[%d] = %d\n", i, b[i]);
    }
    printf("\n");
}

int getSum(int localX[], int localY[], int localN) {
    int localSum = 0;
    for (int i = 0; i < localN; ++i) {
        localSum = localSum + localX[i] * localY[i];
    }
    return localSum;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int N = 10;
    int x[N];
    int y[N];
    int size;
    int rank;
    int sum;
    int localSum;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        generateArray(x, y, N);
    }

    int *a = new int[size];
    int *b = new int[size];
    forScatterv(a, b, N, size);

    int localN = a[rank];
    int *localX = new int[localN];
    int *localY = new int[localN];

    MPI_Scatterv(x, a, b, MPI_INT, localX, localN, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(y, a, b, MPI_INT, localY, localN, MPI_INT, 0, MPI_COMM_WORLD);

    localSum = getSum(localX, localY, localN);

    MPI_Reduce(&localSum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Scalar: %d", sum);
    }

    MPI_Finalize();
}

