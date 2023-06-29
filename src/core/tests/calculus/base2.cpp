

#include "y/calculus/base2.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

template <typename T>
static inline void showBase2(const char *name)
{
    std::cerr << "Base2<" << name << ">" << std::endl;
    std::cerr << "\tMaxShift=" << Base2<T>::MaxShift      << std::endl;
    std::cerr << "\tMaxSlots=" << Base2<T>::MaxSlots      << std::endl;
    std::cerr << "\tMaxPower=" << Base2<T>::MaxPowerOfTwo << std::endl;

}


#define SHOW(CLASS) showBase2<CLASS>(#CLASS)

Y_UTEST(calculus_base2)
{
    SHOW(int32_t);
    SHOW(uint64_t);
}
Y_UDONE()
