cmake_minimum_required(VERSION 3.20)
project(MPI)

set(CMAKE_CXX_STANDARD 14)

add_executable(MPI mpi11.cpp)

find_package(MPI REQUIRED)
target_link_libraries(MPI PUBLIC MPI::MPI_CXX)