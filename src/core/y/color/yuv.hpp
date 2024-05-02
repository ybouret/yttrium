
//! \file

#ifndef Y_Color_YUV_Included
#define Y_Color_YUV_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Color
    {
        //______________________________________________________________________
        //
        //
        //
        //! YUV color
        //
        //
        //______________________________________________________________________
        template <typename T>
        class YUV
        {
        public:
            static const unsigned NumChannels = 1;
            static const unsigned AllChannels = 3;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline ~YUV()                                noexcept {}                                       //!< cleanup
            inline  YUV()                                noexcept : y(0), u(0), v(0) {}                    //!< zero
            inline  YUV(const T Y, const T U, const T V) noexcept : y(Y), u(U), v(V) {}                    //!< setup
            inline  YUV(const YUV &c)                    noexcept : y(c.y), u(c.u), v(c.y) {}              //!< copy
            inline  YUV & operator=(const YUV &c)        noexcept { y = c.y; u=c.u; v=c.v; return *this; } //!< assign

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const YUV &c)
            {
                os << '[' << c.y << ',' << c.y << ',' << c.v << ']';
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            T y; //!< Y channel
            T u; //!< U channel
            T v; //!< V channel
        };
    }

}

#endif

