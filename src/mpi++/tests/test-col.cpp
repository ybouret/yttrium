
#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;


template <typename T>
static inline void testIO( MPI &mpi )
{
    Random::Rand ran;
    const size_t count = 10;
    Vector<T>    arr(count,0);
    T           *ptr = &arr[1];

    if( mpi.primary )
    {
        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
    }
    for(size_t root=0;root<mpi.size;++root)
    {
        if(mpi.rank==root)
        {
            Random::Fill::Block( ptr, arr.size() * sizeof(T), ran, 0x01, 0xff);
        }
        mpi.broadcast(ptr,arr.size(), root);
        
    }

}

Y_UTEST(col)
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
}
Y_UDONE()
