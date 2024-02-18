#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(p2p)
{

    MPI & mpi = MPI::Init(&argc,&argv,MPI_THREAD_SINGLE);


    int data[2] = { 0, 0 };
    if( 0 == mpi.rank )
    {
        data[0] = 1;
        data[1] = 2;
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            mpi.Send(data,2,rank);
        }
    }
    else
    {
        mpi.Recv(data,2,0);
    }
    std::cerr << "@" << mpi.name << " : " << data[0] << ", " << data[1] << std::endl;


}
Y_UDONE()

