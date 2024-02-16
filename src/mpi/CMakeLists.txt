
find_package(MPI)

if(MPI_FOUND)
    include_directories(. ../core)
    Y_CreateLibrary(y-mpi mpi)
    add_subdirectory(tests)
endif()