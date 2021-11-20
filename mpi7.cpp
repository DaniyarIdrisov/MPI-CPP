#include<mpi.h>
#include<iostream>

const int N = 3;

void generateValues(int x[][N], int y[]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            x[i][j] = rand() / 100;
            printf("a[%d][%d] = %d  ", i, j, x[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        y[i] = rand() / 100;
        printf("a[%d] = %d\n", i, y[i]);
    }
    printf("\n");
}

void generateLineMatrix(int x[][N], int lineMatrix[]) {
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++){
            lineMatrix[i * N + j] = x[j][i];
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int a[N][N];
    int b[N];
    int c[N];
    int lineMatrix[N * N];
    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        generateValues(a, b);
        generateLineMatrix(a, lineMatrix);
    }

    int *localArray = new int[N];

    MPI_Scatter(lineMatrix, N, MPI_INT, localArray, N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N; i++) {
        localArray[i] = localArray[i] * b[rank];
    }

    MPI_Reduce(localArray, c, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            printf("c[%d] = %d\n", i, c[i]);
        }
    }

    MPI_Finalize();
}

