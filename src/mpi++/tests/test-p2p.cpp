#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/container/cxx/array.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

namespace Yttrium
{
    
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
            mpi.send(&arr[1],count,rank);
        }
    }
    else
    {
        mpi.recv(&arr[1],count,0);
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

    if(mpi.primary) std::cerr << "Testing Size" << std::endl;
   
    const size_t original = 0xabcdef;
    if( mpi.primary )
    {
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            mpi.sendSize(original,rank);
        }
    }
    else
    {
        const size_t sz = mpi.recvSize(0);
        Y_ASSERT(original==sz);
    }


    if(mpi.primary) std::cerr << "Testing [Send|Recv]One" << std::endl;

    const int ival = 0xabcdef;
    if( mpi.primary )
    {
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            MPI::SendOne<int>::With(mpi,ival,rank);
        }
    }
    else
    {
        const int rval = MPI::RecvOne<int>::With(mpi,0);
        Y_ASSERT(rval==ival);
    }

    const String s = "Hello, World!";
    if( mpi.primary )
    {
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            MPI::SendOne<String>::With(mpi,s,rank);
        }
    }
    else
    {
        const String r = MPI::RecvOne<String>::With(mpi,0);
        Y_ASSERT(r==s);
    }


}
Y_UDONE()

