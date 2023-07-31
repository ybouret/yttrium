
//! \file

#ifndef Y_IO_Output_Stream_Included
#define Y_IO_Output_Stream_Included 1


#include "y/io/stream.hpp"
#include "y/memory/buffer/ro.hpp"
#include "y/check/printf.hpp"

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
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit OutputStream() noexcept; //!< setup

    public:
        virtual ~OutputStream() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual void write(const char) = 0; //!< write one char
        virtual void flush()           = 0; //!< flush cache is any

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! write blocks of memory
        void write(const void *blockAddr, const size_t blockSize);

        OutputStream & operator<<(const char );                      //!< helper
        OutputStream & operator<<(const char *);                     //!< helper
        OutputStream & operator<<(const Memory::ReadOnlyBuffer &);   //!< helper
        OutputStream & operator()(const char *fmt,...) Y_PRINTF_API; //!< helper


        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(OutputStream);
    };
}

#endif

