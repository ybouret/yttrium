Y_CreateTest(mpi++)

Y_LinkLibraries(test-mpi++ y-mpi++)
target_link_libraries(test-mpi++ ${MPI_C_LIBRARIES})
