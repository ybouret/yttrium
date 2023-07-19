
#include "y/type/traits.hpp"
#include "y/utest/run.hpp"

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
        std::cerr << "IsConst    (" << std::setw(16) << typeName << ") => " << Answer(result) << ", expecting " << Answer(expected) << std::endl;
        Y_ASSERT(result==expected);
    }

    template <typename T>
    static inline
    void testPointer(const char *typeName,
                     const bool  expected)
    {
        const bool result = TypeTraits<T>::IsPointer;
        std::cerr << "IsPointer  (" << std::setw(16) << typeName << ") => " << Answer(result) << ", expecting " << Answer(expected) << std::endl;
        Y_ASSERT(result==expected);
    }

    template <typename T>
    static inline
    void testRef(const char *typeName,
                 const bool  expected)
    {
        const bool result = TypeTraits<T>::IsReference;
        std::cerr << "IsReference(" << std::setw(16) << typeName << ") => " << Answer(result) << ", expecting " << Answer(expected) << std::endl;
        Y_ASSERT(result==expected);
    }


}

#define IS_CONST(TYPE,EXPECTED)   testConst<TYPE>(#TYPE,EXPECTED)
#define IS_POINTER(TYPE,EXPECTED) testPointer<TYPE>(#TYPE,EXPECTED)
#define IS_REF(TYPE,EXPECTED)     testRef<TYPE>(#TYPE,EXPECTED)

Y_UTEST(type_traits)
{
    IS_CONST(int,false);
    IS_CONST(const int,true);
    IS_CONST(const void *,true);
    IS_CONST(void *,false);
    IS_CONST(double &,false);
    IS_CONST(const float &,true);

    IS_POINTER(void *,true);
    IS_POINTER(const int,false);

    IS_REF(int,false);
    IS_REF(int &,true);
    IS_REF(const int &,true);
    IS_REF(void *,false);
    IS_REF(void * &, true);
}
Y_UDONE()
