#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;


template <typename T>
static inline void testIO(MPI &mpi)
{
    const size_t n = 100;
    Vector<T>    inp(n,0);
    Vector<T>    out(n,0);

    if( mpi.primary )
    {
        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
    }

    


}

Y_UTEST(xch)
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
