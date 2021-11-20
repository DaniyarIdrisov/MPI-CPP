#include <stdio.h>
#include <cmath>
#include <mpi.h>

const int N = 1000000000;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int size;
    int rank;

    double h;
    double x;
    double sum = 0.0;
    double pi;
    double localPi;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    h = 1.0 / (double) N;
    for (int i = rank + 1; i <= N; i = i + size) {
        x = -(h * (i - 0.5));
        sum = sum + sqrt(1.0 - x * x);
    }

    localPi = h * sum;

    MPI_Reduce(&localPi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = pi * 4;
        printf("Pi = %lf", pi);
    }

    MPI_Finalize();
}
