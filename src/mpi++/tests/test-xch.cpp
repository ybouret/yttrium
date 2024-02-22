#include "y/mpi++/api.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;


template <typename T>
static inline void testIO(MPI &mpi)
{
    Random::Rand ran;
    const size_t n = 10;
    Vector<T>    inp(n,0);
    Vector<T>    out(n,0);

    Random::Fill::Block(out(), out.size() * sizeof(T), ran);


    if( mpi.primary )
    {
        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
    }

    const size_t nextRank = mpi.next1D();
    const size_t prevRank = mpi.prev1D();
    mpi.sendrecv(out(), out.size(), nextRank, MPI::Tag, inp(), inp.size(), prevRank, MPI::Tag);


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
