#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/container/cxx/array.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

template <typename T>
static inline void testIO( MPI &mpi )
{
    const size_t count = 10;
    CxxArray<T>  arr(count);
    if( mpi.primary )
    {
        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
        Random::Rand ran;
        Random::Fill::Block( &arr[1], arr.size() * sizeof(T), ran, 0x01, 0xff);
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            mpi.Send(&arr[1],count,rank);
        }
    }
    else
    {
        mpi.Recv(&arr[1],count,0);
    }
}

Y_UTEST(p2p)
{

    MPI & mpi = MPI::Init(&argc,&argv,MPI_THREAD_SINGLE);

    testIO<char>(mpi);
    testIO<signed char>(mpi);
    testIO<unsigned char>(mpi);
    testIO<float>(mpi);
    testIO<double>(mpi);
    testIO<long double>(mpi);
    



}
Y_UDONE()

