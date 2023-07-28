

//! \file

#ifndef Y_IO_Input_Stream_Included
#define Y_IO_Input_Stream_Included 1


#include "y/io/stream.hpp"

namespace Yttrium
{
    //______________________________________________________________________
    //
    //
    //
    //! Base class for input strean
    //
    //
    //______________________________________________________________________
    class InputStream : public virtual Stream
    {
    protected:
        explicit InputStream() noexcept; //!< setup

    public:
        virtual ~InputStream() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // methods
        //
        //______________________________________________________________________
        virtual bool query(char &C) = 0;



    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputStream);
    };
}

#endif

