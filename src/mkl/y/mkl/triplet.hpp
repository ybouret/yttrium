//! \file

#ifndef Y_MKL_Triple_Included
#define Y_MKL_Triple_Included 1

#include "y/sort/nw.hpp"
#include "y/type/args.hpp"
#include <iostream>

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Triplet of scalar data
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Triplet
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);        //!< aliases
            static const size_t SIZE = 3; //!< alias

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            T a; //!< a
            T b; //!< b
            T c; //!< c

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Triplet &t)
            {
                os << '[' << t.a << ';' << t.b << ';' << t.c << ']';
                return os;
            }

            //! optimized sorting
            inline void makeIncreasing()
            {
                NetworkSort::Algo<SIZE>::Increasing(*this);
            }

            //! optimized co-sorting
            template <typename U>
            inline void makeIncreasingWith(Triplet<U> &other)
            {
                NetworkSort::Algo<SIZE>::Increasing(*this,other);
            }

            //! check if ordinates form a local minimum
            inline bool isLocalMinimum() const
            {
                return (b<=a) && (b<=c);
            }

            //! check increasing values
            inline bool isIncreasing() const {
                return a<=b && b <= c;
            }

            //! check decreasing values
            inline bool isDecreasing() const {
                return c <= b && b <= a;
            }

            //! check ordered value
            inline bool isOrdered() const
            {
                return isIncreasing() || isDecreasing();
            }

            //__________________________________________________________________
            //
            //
            // writable-like interface
            //
            //__________________________________________________________________

            //! helper
            inline size_t size() const noexcept { return SIZE; }

            //! in [1:3]
            inline ConstType & operator[](const size_t indx) const noexcept
            {
                assert(indx>=1); assert(indx<=3);
                return *(( &a - 1) + indx);
            }

            //! in [1:3]
            inline  Type & operator[](const size_t indx)   noexcept
            {
                assert(indx>=1); assert(indx<=3);
                return *(( &a - 1) + indx);
            }

            template <typename U> inline
            Triplet & operator=( const Triplet<U> &_ )
            {
                a = static_cast<T>(_.a);
                b = static_cast<T>(_.b);
                c = static_cast<T>(_.c);
                return *this;
            }

            //! load three next value from array
            template <typename U> inline
            void load(U *arr)
            {
                assert(0!=arr);
                a = arr[0];
                b = arr[1];
                c = arr[2];
            }

            

        };

    }

}

#endif

