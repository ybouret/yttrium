
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/type/bitlib.hpp"
#include "y/random/bits.hpp"


using namespace Yttrium;

template <size_t N>
static inline void displayInts()
{
    std::cerr << "SignedInt<" << N << ">" << std::endl;
    typedef SignedInt<N> SI;
    Y_ASSERT(sizeof(typename SI::Type) == N);
    std::cerr << "\tmin: " << int64_t(SI::Minimum) << std::endl;
    std::cerr << "\tmax: " << int64_t(SI::Maximum) << std::endl;

    std::cerr << "UnsignedInt<" << N << ">" << std::endl;
    typedef UnsignedInt<N> UI;
    Y_ASSERT(sizeof(typename UI::Type) == N);
    std::cerr << "\tmin: " << uint64_t(UI::Minimum) << std::endl;
    std::cerr << "\tmax: " << uint64_t(UI::Maximum) << std::endl;
}

template <typename T>
static inline void displayType(const char *name)
{
    typedef IntegerFor<T> Info;
    std::cerr << name << std::endl;
    std::cerr << "\tBytes  = " << Info::Bytes  << std::endl;
    std::cerr << "\tSigned = " << Info::Signed << std::endl;

    if(Info::Signed)
    {
        std::cerr << "\tmin    = " << int64_t(Info::Minimum) << std::endl;
        std::cerr << "\tmax    = " << int64_t(Info::Maximum) << std::endl;
    }
    else
    {
        std::cerr << "\tmin    = " << uint64_t(Info::Minimum) << std::endl;
        std::cerr << "\tmax    = " << uint64_t(Info::Maximum) << std::endl;
    }

}


#define Y_DISPLAY(CLASS) displayType<CLASS>(#CLASS)

#define LOOPS 100000

Y_UTEST(type_ints)
{

    Random::Rand ran;

    displayInts<1>();
    displayInts<2>();
    displayInts<4>();
    displayInts<8>();

    Y_DISPLAY(char);
    Y_DISPLAY(short);
    Y_DISPLAY(size_t);


    for(int i=-5;i<=5;++i)
    {
        std::cerr << i << " -> " << Clamp(-3,i,3) << std::endl;
    }

    Y_SIZEOF(int);
    Y_SIZEOF(long);
    Y_SIZEOF(long long);

    std::cerr << "LRoll" << std::endl;
    for(uint8_t i=0;i<=200;i+=10)
    {

        BitLib::Display(std::cerr << "\t", i) << " -> ";
        BitLib::Display(std::cerr, BitLib::LRoll(i) ) << std::endl;

    }

    std::cerr << std::endl;
    std::cerr << "BytesFor..." << std::endl;
    {
        std::cerr << "uint8_t" << std::endl;
        std::cerr << "\tzero" << std::endl;
        Y_CHECK(0==RequiredBytesFor<uint8_t>(0));

        std::cerr << "\tnext1" << std::endl;
        for(uint64_t i=1;i<=UnsignedInt<1>::Maxi1;++i)
        {
            Y_ASSERT(1==RequiredBytesFor<uint8_t>( uint8_t(i) ));
        }


    }

    {
        std::cerr << "uint16_t" << std::endl;
        std::cerr << "\tzero" << std::endl;
        Y_CHECK(0==RequiredBytesFor<uint16_t>(0));

        std::cerr << "\tnext1" << std::endl;
        for(uint64_t i=0x01;i<=UnsignedInt<2>::Maxi1;++i)
        {
            Y_ASSERT(1==RequiredBytesFor<uint16_t>( uint16_t(i) ));
        }

        std::cerr << "\tnext2" << std::endl;
        for(uint64_t i=UnsignedInt<1>::Maxi1+1;i<=UnsignedInt<2>::Maxi2;++i)
        {
            Y_ASSERT(2==RequiredBytesFor<uint16_t>( uint16_t(i) ));
        }

    }

    {
        std::cerr << "uint32_t" << std::endl;
        std::cerr << "\tzero" << std::endl;
        Y_CHECK(0==RequiredBytesFor<uint32_t>(0));

        std::cerr << "\tnext1" << std::endl;
        for(uint64_t i=0x01;i<=UnsignedInt<4>::Maxi1;++i)
        {
            Y_ASSERT(1==RequiredBytesFor<uint32_t>( uint32_t(i) ));
        }

        std::cerr << "\tnext2" << std::endl;
        for(uint64_t i=UnsignedInt<4>::Maxi1+1;i<=UnsignedInt<4>::Maxi2;++i)
        {
            Y_ASSERT(2==RequiredBytesFor<uint32_t>( uint32_t(i) ));
        }

        std::cerr << "\tnext3" << std::endl;
        for(uint64_t i=UnsignedInt<4>::Maxi2+1;i<=UnsignedInt<4>::Maxi3;++i)
        {
            Y_ASSERT(3==RequiredBytesFor<uint32_t>( uint32_t(i) ));
        }

        std::cerr << "\tnext4" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint32_t u = ran.in<uint32_t>(UnsignedInt<4>::Maxi3+1,UnsignedInt<4>::Maxi4);
            Y_ASSERT(4==RequiredBytesFor<uint32_t>( u));
        }
    }

    {
        std::cerr << "uint64_t" << std::endl;
        std::cerr << "\tzero" << std::endl;
        Y_CHECK(0==RequiredBytesFor<uint64_t>(0));

        std::cerr << "\tnext1" << std::endl;
        for(uint64_t i=0x01;i<=UnsignedInt<8>::Maxi1;++i)
        {
            Y_ASSERT(1==RequiredBytesFor<uint64_t>( i ));
        }

        std::cerr << "\tnext2" << std::endl;
        for(uint64_t i=UnsignedInt<8>::Maxi1+1;i<=UnsignedInt<8>::Maxi2;++i)
        {
            Y_ASSERT(2==RequiredBytesFor<uint64_t>( i ));
        }

        std::cerr << "\tnext3" << std::endl;
        for(uint64_t i=UnsignedInt<8>::Maxi2+1;i<=UnsignedInt<8>::Maxi3;++i)
        {
            Y_ASSERT(3==RequiredBytesFor<uint64_t>( i ));
        }

        std::cerr << "\tnext4" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint64_t u = ran.in<uint32_t>(UnsignedInt<8>::Maxi3+1,UnsignedInt<8>::Maxi4);
            Y_ASSERT(4==RequiredBytesFor<uint64_t>( u));
        }

        std::cerr << "\tnext5" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint64_t u = ran.in<uint64_t>(UnsignedInt<8>::Maxi4+1,UnsignedInt<8>::Maxi5);
            Y_ASSERT(5==RequiredBytesFor<uint64_t>( u));
        }

        std::cerr << "\tnext6" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint64_t u = ran.in<uint64_t>(UnsignedInt<8>::Maxi5+1,UnsignedInt<8>::Maxi6);
            Y_ASSERT(6==RequiredBytesFor<uint64_t>( u));
        }

        std::cerr << "\tnext7" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint64_t u = ran.in<uint64_t>(UnsignedInt<8>::Maxi6+1,UnsignedInt<8>::Maxi7);
            Y_ASSERT(7==RequiredBytesFor<uint64_t>( u));
        }

        std::cerr << "\tnext8" << std::endl;
        for(size_t iter=0;iter<LOOPS;++iter)
        {
            const uint64_t u = ran.in<uint64_t>(UnsignedInt<8>::Maxi7+1,UnsignedInt<8>::Maxi8);
            Y_ASSERT(8==RequiredBytesFor<uint64_t>( u));
        }
    }

    


}
Y_UDONE()
