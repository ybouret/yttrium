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

            //__________________________________________________________________
            //
            //
            //
            //! Proxy to handle
            /**
             - float, double, long double
             - XReal<float|double|long double>
             - and any Complex based on types above...
             */
            //
            //
            //__________________________________________________________________
            template <typename T>
            struct Proxy
            {
                //______________________________________________________________
                //
                //! alias to hold Fabs
                //______________________________________________________________
                typedef typename  ScalarFor<T>::Type Scalar;

                //______________________________________________________________
                //
                //
                //! Unit to hold value and Fabs(value)
                //
                //______________________________________________________________
                class Unit
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________

                    //! setup from given value
                    inline Unit(const T &args) noexcept:
                    usrValue(args), absValue( MKL::Fabs<T>::Of(usrValue) )
                    { }

                    //! cleanup
                    inline ~Unit() noexcept {}

                    //! copy
                    inline Unit(const Unit &other) noexcept:
                    usrValue(other.usrValue), absValue(other.absValue)
                    { }

                    //__________________________________________________________
                    //
                    // Methods
                    //__________________________________________________________

                    //! retrieve original value by cast
                    inline operator T() const noexcept
                    {
                        return usrValue;
                    }

                    //! display original value
                    inline friend std::ostream & operator<<(std::ostream &os, const Unit &self)
                    {
                        os << self.usrValue;
                        return os;
                    }

                    //! addition
                    inline friend Unit operator+(const Unit &lhs, const Unit &rhs) noexcept
                    {
                        const T sum = lhs.usrValue + rhs.usrValue;
                        return Unit(sum);
                    }

                    //__________________________________________________________
                    //
                    // Members
                    //__________________________________________________________
                    const T      usrValue; //!< user's value
                    const Scalar absValue; //!< Fabs(usrValue)

                private:
                    Y_DISABLE_ASSIGN(Unit);
                };

                //______________________________________________________________
                //
                //! alias to convert Unit to argument to Policy
                //______________________________________________________________
                typedef const Unit       UnitArgs;

                //______________________________________________________________
                //
                //! need to use an internal heap
                //______________________________________________________________
                typedef HeapPolicy<Unit> Policy;

                //______________________________________________________________
                //
                //! memory model for flexible units with ALLOCATOR
                //______________________________________________________________
                template <typename ALLOCATOR>
                struct FlexibleUnits {
                    typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type; //!< alias
                };

                //______________________________________________________________
                //
                //! memory model for N compiled units
                //______________________________________________________________
                template <size_t N>
                struct CompiledUnits {
                    typedef typename Policy:: template CompiledUnits<N>::Type Type; //!< alias
                };


            };

            //__________________________________________________________________
            //
            //
            //
            //! Proxy to handle APQ
            //
            //
            //__________________________________________________________________
            template <>
            struct Proxy<apq> {

                typedef apq              Unit;      //!< apq is its self unit
                typedef const Unit &     UnitArgs;  //!< no convertion
                typedef DirePolicy<Unit> Policy;    //!< user dire units

                //______________________________________________________________
                //
                //! memory model for flexible units with ALLOCATOR
                //______________________________________________________________
                template <typename ALLOCATOR>
                struct FlexibleUnits {
                    typedef typename Policy::template FlexibleUnits<ALLOCATOR>::Type Type; //!< alias
                };

                //______________________________________________________________
                //
                //! memory model for N compiled units
                //______________________________________________________________
                template <size_t N>
                struct CompiledUnits
                {
                    typedef typename Policy:: template CompiledUnits<N>::Type Type; //!< alias
                };
            };

        }
    }

}

#endif

