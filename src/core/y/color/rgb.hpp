//! \file

#ifndef Y_Color_RGB_Included
#define Y_Color_RGB_Included 1

#include "y/color/print.hpp"

namespace Yttrium
{
    namespace Color
    {
        template <typename> class RGBA;

        //______________________________________________________________________
        //
        //
        //
        //! RGB color
        //
        //
        //______________________________________________________________________
        template <typename T>
        class RGB
        {
        public:
            typedef T Type;                        //!< alias
            static const unsigned NumChannels = 3; //!< alias
            static const unsigned AllChannels = 3; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline ~RGB()                                noexcept {}                                        //!< cleanup
            inline  RGB()                                noexcept : r(0), g(0), b(0) {}                     //!< zero
            inline  RGB(const T R, const T G, const T B) noexcept : r(R), g(G), b(B) {}                     //!< setup
            inline  RGB(const RGB &c)                    noexcept : r(c.r), g(c.g), b(c.b) {}               //!< copy
            inline  RGB & operator=(const RGB &c)        noexcept { r = c.r; g=c.g; b=c.b; return *this; }  //!< assign
            inline  RGB(const RGBA<T> &)                 noexcept;                                          //!< copy r,g,b
            inline  RGB & operator=(const RGBA<T> &)     noexcept;                                          //!< assign r,g,b

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const RGB &c)
            {
                os << '['; Print<T>::Out(os,c.r);
                os << ','; Print<T>::Out(os,c.g);
                os << ','; Print<T>::Out(os,c.b);
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
        };

        
    }

}

#endif

