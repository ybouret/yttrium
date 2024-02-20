#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

namespace Yttrium
{
    
}

#if 1
template <typename T>
static inline void testIO( MPI &mpi )
{
    const size_t count = 10;
    Vector<T>    arr(count,0);
    T           *ptr = &arr[1];
    
    if( mpi.primary )
    {
        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
        Random::Rand ran;
        Random::Fill::Block( ptr, arr.size() * sizeof(T), ran, 0x01, 0xff);
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            mpi.send(ptr,count,rank,MPI::Tag);
        }
    }
    else
    {
        mpi.recv(ptr,count,0,MPI::Tag);
    }
}
#endif

Y_UTEST(p2p)
{

    MPI & mpi = MPI::Init(&argc,&argv,MPI_THREAD_SINGLE);


#if 1
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
#endif

    if(mpi.primary) std::cerr << "Testing Size" << std::endl;
   
    const size_t original = 0xabcdef;
    if( mpi.primary )
    {
        for(size_t rank=1;rank<mpi.size;++rank)
        {
            mpi.sendSize(original,rank,MPI::Tag);
        }
    }
    else
    {
        const size_t sz = mpi.recvSize(0,MPI::Tag);
        Y_ASSERT(original==sz);
    }


    if(mpi.primary) std::cerr << "Testing [Send|Recv]One" << std::endl;

    {
        const int ival = 0xabcdef;
        if( mpi.primary )
        {
            for(size_t rank=1;rank<mpi.size;++rank)
            {
                MPI::SendOne<int>::With(mpi,ival,rank,MPI::Tag);
            }
        }
        else
        {
            const int rval = MPI::RecvOne<int>::With(mpi,0,MPI::Tag);
            Y_ASSERT(rval==ival);
        }
    }

    {
        const String s = "Hello, World!";
        if( mpi.primary )
        {
            for(size_t rank=1;rank<mpi.size;++rank)
            {
                MPI::SendOne<String>::With(mpi,s,rank,MPI::Tag);
            }
        }
        else
        {
            const String r = MPI::RecvOne<String>::With(mpi,0,MPI::Tag);
            Y_ASSERT(r==s);
        }
    }


}
Y_UDONE()

