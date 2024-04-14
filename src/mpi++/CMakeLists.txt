
message( STATUS "Looking for MPI implementation, please wait..." )

find_package(MPI)

if(MPI_C_FOUND)
    include_directories(. ../core)
    Y_CreateLibrary(y-mpi++ 
    mpi++
    mpi++/api
    )
    message( STATUS "[yttrium] '${MPI_C_INCLUDE_DIRS}'")
    include_directories(${MPI_C_INCLUDE_DIRS})
    add_subdirectory(tests)
endif()
