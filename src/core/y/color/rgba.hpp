
//! \file

#ifndef Y_Color_RGBA_Included
#define Y_Color_RGBA_Included 1

#include "y/color/print.hpp"
#include "y/color/opaque.hpp"

namespace Yttrium
{
    namespace Color
    {
        template <typename T>
        class RGBA
        {
        public:

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline ~RGBA()                                 noexcept {}                                        //!< cleanup
            inline  RGBA(const T A = Opaque<T>::Value)     noexcept : r(0), g(0), b(0), a(A) {}                     //!< zero
            inline  RGBA(const T R, const T G, const T B, const T A = Opaque<T>::Value)  noexcept : r(R), g(G), b(B), a(A) {}                     //!< setup
            inline  RGBA(const RGBA &c)                    noexcept : r(c.r), g(c.g), b(c.b), a(c.a) {}               //!< copy
            inline  RGBA & operator=(const RGBA &c)        noexcept { r = c.r; g=c.g; b=c.b; a=c.a; return *this; }  //!< assign

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const RGBA &c)
            {
                os << '['; Print<T>::Out(os,c.r);
                os << ','; Print<T>::Out(os,c.g);
                os << ','; Print<T>::Out(os,c.b);
                os << ','; Print<T>::Out(os,c.a);
                os << ']';
                return os;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            T r; //!< red
            T g; //!< green
            T b; //!< blue
            T a; //!< alpha

        };


    }

}

#endif
