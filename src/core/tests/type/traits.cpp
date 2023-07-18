
#include "y/type/traits.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace Yttrium;

namespace
{
    static const char *Answer(const bool flag) noexcept
    {
        return flag ? "YES" : "NO ";
    }

    template <typename T>
    static inline
    void testConst(const char *typeName,
                   const bool  expected)
    {
        const bool result = TypeTraits<T>::IsConst;
        std::cerr << "IsConst(" << std::setw(16) << typeName << ")= " << Answer(result) << ", expecting " << Answer(expected) << std::endl;
    }

}

#define IS_CONST(TYPE,EXPECTED) testConst<TYPE>(#TYPE,EXPECTED)

Y_UTEST(type_traits)
{
    IS_CONST(int,false);
    IS_CONST(const int,true);

}
Y_UDONE()
