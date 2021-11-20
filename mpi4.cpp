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

void generateArray(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = rand() / 100;
        printf("a[%d] = %d\n", i, a[i]);
    }
    printf("\n");
}

double getAverage(int localArray[], int localN) {
    int sum = 0;
    int count = 0;
    for (int i = 0; i < localN; ++i) {
        if (localArray[i] > 0) {
            sum = sum + localArray[i];
            count = count + 1;
        }
    }
    return (double) (sum / count);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int N = 10;
    int array[N];
    int size;
    int rank;
    double average;
    double localAverage;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        generateArray(array, N);
    }

    int *a = new int[size];
    int *b = new int[size];
    forScatterv(a, b, N, size);

    int localN = a[rank];
    int *localArray = new int[localN];

    MPI_Scatterv(array, a, b, MPI_INT, localArray, localN, MPI_INT, 0, MPI_COMM_WORLD);

    localAverage = getAverage(localArray, localN);

    MPI_Reduce(&localAverage, &average, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        average = average / (double) size;
        printf("Average: %f", average);
    }

    MPI_Finalize();
}

