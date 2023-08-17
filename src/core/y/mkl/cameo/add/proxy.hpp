//! \file

#ifndef Y_CamEO_Add_Proxy_Included
#define Y_CamEO_Add_Proxy_Included 1


#include "y/mkl/cameo/add/heap.hpp"
#include "y/mkl/cameo/add/dire.hpp"
#include "y/mkl/scalar.hpp"
#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Cameo
        {


            template <typename T>
            struct Proxy
            {
                typedef typename  ScalarFor<T>::Type Scalar;

                class Unit
                {
                public:
                    const T      usrValue;
                    const Scalar absValue;
                    inline Unit(const T &args) noexcept:
                    usrValue(args),
                    absValue( MKL::Fabs<T>::Of(usrValue) )
                    {
                    }

                    inline ~Unit() noexcept {}

                    inline Unit(const Unit &other) noexcept:
                    usrValue(other.usrValue),
                    absValue(other.absValue)
                    {
                    }

                    inline operator T() const noexcept
                    {
                        return usrValue;
                    }

                    inline friend std::ostream & operator<<(std::ostream &os, const Unit &self)
                    {
                        os << self.usrValue; //<< '@' << '|' << self.absValue << '|';
                        return os;
                    }

                    inline friend Unit operator+(const Unit &lhs, const Unit &rhs) noexcept
                    {
                        const T sum = lhs.usrValue + rhs.usrValue;
                        return Unit(sum);
                    }

                private:
                    Y_DISABLE_ASSIGN(Unit);
                };

                typedef const Unit       UnitArgs;
                typedef HeapPolicy<Unit> Policy;

                template <typename ALLOCATOR>
                struct FlexibleUnits {
                    typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type;
                };

                template <size_t N>
                struct CompiledUnits {
                    typedef typename Policy:: template CompiledUnits<N>::Type Type;
                };


            };

            template <>
            struct Proxy<apq> {
                typedef apq Unit;

                typedef const Unit &     UnitArgs;
                typedef DirePolicy<Unit> Policy;

                template <typename ALLOCATOR>
                struct FlexibleUnits {
                    typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type;
                };

                template <size_t N>
                struct CompiledUnits
                {
                    typedef typename Policy:: template CompiledUnits<N>::Type Type;
                };
            };

        }
    }

}

#endif

