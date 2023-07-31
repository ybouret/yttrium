

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
    //! Base class for input stream
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
        // Interface
        //
        //______________________________________________________________________
        virtual bool query(char & data) = 0; //!< query a single char, false = EOS
        virtual void store(const  char) = 0; //!< unread a char
        virtual bool ready()            = 0; //!< next char ?

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        static const char *From(const char *ctx) noexcept; //!< ctx/Unknown

        size_t fetch(uint8_t  &); //!< try to read 1 byte
        size_t fetch(uint16_t &); //!< try to read 2 bytes
        size_t fetch(uint32_t &); //!< try to read 4 bytes
        size_t fetch(uint64_t &); //!< try to read 8 bytes




        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputStream);
    };
}

#endif

