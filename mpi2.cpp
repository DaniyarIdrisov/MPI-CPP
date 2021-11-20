#include<mpi.h>
#include<iostream>

const int N = 5;

int findMax(int x[], int localN) {
    int max;
    max = x[0];
    for (int i = 0; i < localN; ++i) {
        if (x[i] > max) {
            max = x[i];
        }
    }
    return max;
}

void generateArray(int a[]) {
    for (int i = 0; i < N; i++) {
        a[i] = rand() / 100;
        printf("a[%d] = %d\n", i, a[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int array[N];
    int size;
    int rank;
    int max;
    int localMax;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        generateArray(array);
    }

    int localN = N / size;
    int *localArray = new int[localN];

    MPI_Scatter(array, localN, MPI_INT, localArray, localN, MPI_INT, 0, MPI_COMM_WORLD);

    localMax = findMax(localArray, localN);

    MPI_Reduce(&localMax, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Max of array = %d \n", max);
    }

    MPI_Finalize();
}