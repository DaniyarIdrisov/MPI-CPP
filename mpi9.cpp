#include<mpi.h>
#include<iostream>

void generateArray(int a[], int N) {
    for (int i = 0; i < N; i++) {
        a[i] = rand() / 100;
        printf("a[%d] = %d\n", i, a[i]);
    }
    printf("\n");
}

void forScatterv(int a[], int b[], int bias[], int size, int N) {
    int n = N;
    int localN = n / size;
    a[0] = localN;
    b[0] = 0;
    bias[0] = n - localN;
    for (int i = 1; i < size; i++) {
        n -= localN;
        localN = n / (size - i);
        a[i] = localN;
        b[i] = b[i - 1] + a[i - 1];
        bias[i] = bias[i - 1] - a[i];
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int N = 10;
    int size;
    int rank;
    int* a = new int[N];
    int* inverted = new int[N];

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        generateArray(a, N);
    }

    int* x = new int[size];
    int* y = new int[size];
    int* bias = new int[size];
    forScatterv(x, y, bias, size, N);

    int localN = x[rank];
    int* localArray = new int[localN];

    MPI_Scatterv(a, x, y, MPI_INT, localArray, localN, MPI_INT, 0, MPI_COMM_WORLD);

    int* localInverted = new int[localN];
    for(int i = 0; i < localN; i++) {
        localInverted[i] = localArray[localN - i - 1];
    }

    MPI_Gatherv(localInverted, localN, MPI_INT, inverted, x, bias, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            printf("a[%d] = %d\n", i, inverted[i]);
    }

    MPI_Finalize();
}

