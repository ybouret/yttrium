//! \file

#ifndef Y_Color_RGB_Included
#define Y_Color_RGB_Included 1

#include "y/color/print.hpp"

namespace Yttrium
{
    namespace Color
    {
        template <typename T>
        class RGB
        {
        public:
            inline ~RGB()                                noexcept {}
            inline  RGB()                                noexcept : r(0), g(0), b(0) {}
            inline  RGB(const T R, const T G, const T B) noexcept : r(R), g(G), b(B){}
            inline  RGB(const RGB &c)                    noexcept : r(c.r), g(c.g), b(c.b) {}
            inline  RGB & operator=(const RGB &c)        noexcept { r = c.r; g=c.g; b=c.b; return *this; }

            inline friend std::ostream & operator<<(std::ostream &os, const RGB &c)
            {
                os << '['; Print<T>::Out(os,c.r);
                os << ','; Print<T>::Out(os,c.g);
                os << ','; Print<T>::Out(os,c.b);
                os << ']';
                return os;
            }

            
            T r;
            T g;
            T b;
        };
    }

}

#endif

