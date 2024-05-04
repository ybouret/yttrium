
//! \file

#ifndef Y_Ink_Filter_Scharr_Included
#define Y_Ink_Filter_Scharr_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            //! Scharr3X
            struct Scharr3X
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };
            
            //! Scharr3Y
            struct Scharr3Y
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };
            
            //! Scharr3
            struct Scharr3
            {
                static const char * const Name; //!< name
                typedef Scharr3X X;             //!< alias
                typedef Scharr3Y Y;             //!< alias
            };


            //! Scharr5X
            struct Scharr5X
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };
            
            //! Scharr5Y
            struct Scharr5Y
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };
            
            //! Scharr5
            struct Scharr5
            {
                static const char * const Name; //!< name
                typedef Scharr5X X;             //!< alias
                typedef Scharr5Y Y;             //!< alias
            };
        }
    }

}

#endif

