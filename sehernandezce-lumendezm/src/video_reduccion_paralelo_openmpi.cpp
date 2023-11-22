#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

    std::cout << "Hello, World! from process " << rank << " of " << size
              << " on " << processor_name << std::endl;

    MPI_Finalize();

    return 0;
}

/*
mpic++ -o video_reduccion_parallelo_openmpi video_reduccion_parallelo_openmpi.cpp
mpirun -np 4 ./video_reduccion_parallelo_openmpis
*/