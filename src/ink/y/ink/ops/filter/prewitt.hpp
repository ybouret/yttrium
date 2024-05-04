
//! \file

#ifndef Y_Ink_Filter_Prewitt_Included
#define Y_Ink_Filter_Prewitt_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            //! Prewitt3X
            struct Prewitt3X
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };

            //! Prewitt3Y
            struct Prewitt3Y
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };

            struct Prewitt3
            {
                static const char * const Name;
                typedef Prewitt3X         X;
                typedef Prewitt3Y         Y;
            };


            //! Prewitt5X
            struct Prewitt5X
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };

            //! Prewitt5Y
            struct Prewitt5Y
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };

            struct Prewitt5
            {
                static const char * const Name;
                typedef Prewitt5X         X;
                typedef Prewitt5Y         Y;
            };


            //! Prewitt7X
            struct Prewitt7X
            {
                static const char * const Name;       //!< name
                static const int          Data[7][7]; //!< data
            };

            //! Prewitt7Y
            struct Prewitt7Y
            {
                static const char * const Name;       //!< name
                static const int          Data[7][7]; //!< data
            };

            struct Prewitt7
            {
                static const char * const Name;
                typedef Prewitt7X         X;
                typedef Prewitt7Y         Y;
            };
        }

    }


}

#endif

