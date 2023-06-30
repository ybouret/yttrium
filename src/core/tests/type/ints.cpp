
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"


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

Y_UTEST(type_ints)
{
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

}
Y_UDONE()
