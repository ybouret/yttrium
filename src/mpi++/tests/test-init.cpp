#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(init)
{

    MPI & Y_MPI = MPI::Init(&argc,&argv,MPI_THREAD_SINGLE);
    
    std::cerr << Y_MPI.name << " @" << Y_MPI.processorName << std::endl;

    
}
Y_UDONE()

