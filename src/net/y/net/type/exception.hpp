
//! \file

#ifndef Y_Net_Exception_Included
#define Y_Net_Exception_Included 1


#include "y/system/exception.hpp"


namespace Yttrium
{

    namespace Network
    {
#if defined(Y_BSD)
        typedef Libc::Exception  Exception; //!< alias
#endif

#if defined(Y_WIN)
        typedef Win32::Exception Exception; //!< alias
#endif

    }

}

#endif

