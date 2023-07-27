
//! \file

#ifndef Y_IO_Output_Stream_Included
#define Y_IO_Output_Stream_Included 1


#include "y/io/stream.hpp"

namespace Yttrium
{
    //______________________________________________________________________
    //
    //
    //
    //! Base class for output strean
    //
    //
    //______________________________________________________________________
    class OutputStream : public virtual Stream
    {
    protected:
        explicit OutputStream() noexcept; //!< setup

    public:
        virtual ~OutputStream() noexcept; //!< cleanup

        virtual void write(char) = 0;
        virtual void flush()     = 0;
        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputStream);
    };
}

#endif

