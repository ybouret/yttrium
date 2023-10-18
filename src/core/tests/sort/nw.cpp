
#include "y/sort/nw.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/mkl/v3d.hpp"
#include "y/comparison.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "../main.hpp"

using namespace Yttrium;

namespace
{
    template <size_t N, typename T> static inline
    void testNW(Random::Bits &ran)
    {
        std::cerr << "-- N = " << N << std::endl;
        CxxArray<T,Memory::Dyadic> arr(N);
        for(size_t i=1;i<=N;++i)
        {
            arr[i] = Bring<T>::Get(ran);
        }

        std::cerr << "arr=" << arr << std::endl;
        NetworkSort::Algo<N>::Increasing(arr);
        std::cerr << "inc=" << arr << std::endl;
        Y_CHECK(Comparison::IsIncreasing(arr,N));

        NetworkSort::Algo<N>::Decreasing(arr);
        std::cerr << "dec=" << arr << std::endl;
        Y_CHECK(Comparison::IsDecreasing(arr,N));

        std::cerr << std::endl;
    }
}

#define DO_TEST(N) testNW<N,uint16_t>(ran); testNW<N,apq>(ran);


Y_UTEST(sort_nw)
{

    Random::Rand ran;
    std::cerr << "MaxSize=" << NetworkSort::Tables::MaxSize << std::endl;

    for(unsigned i=NetworkSort::Tables::MinSize;i<=NetworkSort::Tables::MaxSize;++i)
    {
        const NetworkSort::Table &table = NetworkSort::Tables::CxxData[i];
        std::cerr << "@" << std::setw(2) << i << " -> #swaps=" << std::setw(4) << table.swaps << std::endl;
    }

    {
        NetworkSort::Common::Display<2>();
        NetworkSort::Common::Display<3>();
        NetworkSort::Common::Display<4>();
        NetworkSort::Common::Display<5>();
        NetworkSort::Common::Display<6>();
        NetworkSort::Common::Display<7>();
        NetworkSort::Common::Display<8>();
        NetworkSort::Common::Display<9>();
        NetworkSort::Common::Display<10>();
        NetworkSort::Common::Display<11>();
        NetworkSort::Common::Display<12>();
        NetworkSort::Common::Display<13>();
        NetworkSort::Common::Display<14>();
        NetworkSort::Common::Display<15>();
        NetworkSort::Common::Display<16>();
        NetworkSort::Common::Display<17>();
        NetworkSort::Common::Display<18>();
        NetworkSort::Common::Display<19>();
        NetworkSort::Common::Display<20>();
        NetworkSort::Common::Display<21>();
        NetworkSort::Common::Display<22>();
        NetworkSort::Common::Display<23>();
        NetworkSort::Common::Display<24>();
        NetworkSort::Common::Display<25>();
        NetworkSort::Common::Display<26>();
        NetworkSort::Common::Display<27>();
        NetworkSort::Common::Display<28>();
        NetworkSort::Common::Display<29>();
        NetworkSort::Common::Display<30>();
        NetworkSort::Common::Display<31>();
        NetworkSort::Common::Display<32>();
    }

    std::cerr << std::endl;
    {
        DO_TEST(2);
        DO_TEST(3);
        DO_TEST(4);
        DO_TEST(5);
        DO_TEST(6);
        DO_TEST(7);
        DO_TEST(8);
        DO_TEST(9);
        DO_TEST(10);
        DO_TEST(11);
        DO_TEST(12);
        DO_TEST(13);
        DO_TEST(14);
        DO_TEST(15);
        DO_TEST(16);
        DO_TEST(17);
        DO_TEST(18);
        DO_TEST(29);
        DO_TEST(20);
        DO_TEST(21);
        DO_TEST(22);
        DO_TEST(23);
        DO_TEST(24);
        DO_TEST(25);
        DO_TEST(26);
        DO_TEST(27);
        DO_TEST(28);
        DO_TEST(29);
        DO_TEST(30);
        DO_TEST(31);
        DO_TEST(32);
    }

}
Y_UDONE()
