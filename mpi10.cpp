#include<mpi.h>
#include<iostream>

void generateArray(int a[], int N) {
    for (int i = 0; i < N; i++) {
        a[i] = rand() / 100;
        printf("a[%d] = %d\n", i, a[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int N = 10;
    int size;
    int rank;
    int a[N];

    double start;
    double end;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        generateArray(a, N);
    }

    if (rank == 0) {

        start = MPI_Wtime();
        MPI_Send(a, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(a, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end= MPI_Wtime();

        printf("RUNTIME 'send' = %f\n", end-start);

        start = MPI_Wtime();
        MPI_Ssend(a, N, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(a, N, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();

        printf("RUNTIME 'ssend': %f\n", end-start);

        start = MPI_Wtime();
        MPI_Rsend(a, N, MPI_INT, 1, 2, MPI_COMM_WORLD);
        MPI_Recv(a, N, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();

        printf("RUNTIME 'rsend': %f\n", end-start);

    } else {
        MPI_Recv(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(a, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Ssend(a, N, MPI_INT, 0, 1, MPI_COMM_WORLD);

        MPI_Recv(a, N, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Rsend(a, N, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

