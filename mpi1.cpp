#include<mpi.h>
#include<iostream>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    printf("Hello world!\n");
    MPI_Finalize();
}
