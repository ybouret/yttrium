
#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(init)
{
    std::cerr << MPI_MAX_ERROR_STRING << std::endl;


    MPI_Init(&argc, &argv);

    MPI::Exception excp(MPI_ERR_TOPOLOGY,"just for %s",program);
    excp.display();
    
    MPI_Finalize();
}
Y_UDONE()

