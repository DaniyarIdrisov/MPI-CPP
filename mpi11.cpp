#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double start;
    double end;
    start = MPI_Wtime();

    int size;
    int rank;
    int sendv;
    int recv;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        sendv = 1;
        printf("START: %d\n", sendv);
        MPI_Send(&sendv, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
    }
    if (rank != 0) {
        MPI_Recv(&recv, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        sendv = 10 * recv;

        if (rank != size - 1) {
            printf("%d\n", sendv);
            MPI_Send(&sendv, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        }
        else {
            printf("%d\n", sendv);
            MPI_Send(&sendv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    if (rank == 0) {
        MPI_Recv(&recv, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        printf("FINISH = %d\n", recv);
    }

    end = MPI_Wtime();
    MPI_Finalize();
    printf("Runtime = %f", end - start);
}

