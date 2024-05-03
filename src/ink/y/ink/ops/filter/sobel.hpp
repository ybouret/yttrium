//! \file

#ifndef Y_Ink_Filter_Sobel_Included
#define Y_Ink_Filter_Sobel_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //! Sobel3X
        struct Sobel3X
        {
            static const char * const Name;       //!< name
            static const int          Data[3][3]; //!< data
        };

        //! Sobel3X
        struct Sobel3Y
        {
            static const char * const Name;       //!< name
            static const int          Data[3][3]; //!< data
        };

        //! Sobel
        struct Sobel
        {
            typedef Sobel3X X; //!< alias
            typedef Sobel3Y Y; //!< alias
        };


    }

    namespace Ink
    {
        //! Sobel5X
        struct Sobel5X
        {
            static const char * const Name;       //!< name
            static const int          Data[5][5]; //!< data
        };

        //! Sobel5Y
        struct Sobel5Y
        {
            static const char * const Name;       //!< name
            static const int          Data[5][5]; //!< data
        };

    }

    namespace Ink
    {
        //! Sobel7X
        struct Sobel7X
        {
            static const char * const Name;       //!< name
            static const int          Data[7][7]; //!< data
        };

        //! Sobel7Y
        struct Sobel7Y
        {
            static const char * const Name;       //!< name
            static const int          Data[7][7]; //!< data
        };

    }




}

#endif

