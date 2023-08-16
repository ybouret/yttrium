#include "../main.hpp"

#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace CamEO
    {
        template <typename T>
        struct Proxy
        {
            class Unit
            {
            public:
                const T usrValue;
                const T absValue;
                inline Unit(const T &args) :
                usrValue(args),
                absValue( MKL::Fabs<T>::Of(usrValue) )
                {
                }

                inline ~Unit() noexcept {}

                inline Unit(const Unit &other) :
                usrValue(other.usrValue),
                absValue(other.absValue)
                {
                }

            private:
                Y_DISABLE_ASSIGN(Unit);
            };

            class Comparator
            {
            public:
                inline  Comparator() noexcept {}
                inline ~Comparator() noexcept {}

                inline SignType operator()(const Unit &lhs, const Unit &rhs)
                {
                    return Comparison::CxxIncreasing(lhs.absValue,rhs.absValue);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Comparator);
            };
        };

        template <>
        struct Proxy<apn> {
            typedef apn Unit;
        };


    }

}

Y_UTEST(mkl_cameo)
{

}
Y_UDONE()

