//! \file

#ifndef Y_MKL_Triple_Included
#define Y_MKL_Triple_Included 1

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
            Y_ARGS_EXPOSE(T,Type); //!< aliases

            T a; //!< a
            T b; //!< b
            T c; //!< c

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Triplet &t)
            {
                os << '[' << t.a << ';' << t.b << ';' << t.c << ']';
                return os;
            }

            //! check if ordinates are minima
            inline bool isMinima() const
            {
                return (b<=a) && (b<=c);
            }

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


        };

    }

}

#endif

