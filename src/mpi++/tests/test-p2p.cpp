#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/container/cxx/array.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

namespace Yttrium
{
    void MPI::SendOne<String>::With(MPI & , const String & , const size_t  , const int  )
    {
    }
}

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
    
    testIO<uint8_t>(mpi);
    testIO<uint16_t>(mpi);
    testIO<uint32_t>(mpi);
    testIO<uint64_t>(mpi);

    testIO<int8_t>(mpi);
    testIO<int16_t>(mpi);
    testIO<int32_t>(mpi);
    testIO<int64_t>(mpi);

    testIO<size_t>(mpi);
    testIO<ptrdiff_t>(mpi);


    if(mpi.primary)
    {
        const String s = "hello";
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            MPI::SendOne<String>::With(mpi,s,rank);
        }
    }

}
Y_UDONE()

