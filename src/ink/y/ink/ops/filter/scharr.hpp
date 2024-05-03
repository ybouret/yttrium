
//! \file

#ifndef Y_Ink_Filter_Scharr_Included
#define Y_Ink_Filter_Scharr_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
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

    }

    namespace Ink
    {
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

    }

}

#endif

