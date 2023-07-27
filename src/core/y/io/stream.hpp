//! \file

#ifndef Y_IO_Stream_Included
#define Y_IO_Stream_Included 1


#include "y/config/starting.hpp"

namespace Yttrium
{
    //______________________________________________________________________
    //
    //
    //
    //! base class for streams
    //
    //
    //______________________________________________________________________
    class Stream
    {
    protected:
        explicit Stream() noexcept; //!< setup
    public:
        virtual ~Stream() noexcept; //!< cleanup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Stream);
    };
    
}

#endif

