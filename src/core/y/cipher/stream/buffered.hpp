
//! \file

#ifndef Y_BufferedStreamCipher_Included
#define Y_BufferedStreamCipher_Included 1

#include "y/cipher/stream.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Stream Cipher using an internal IO::Chars buffer
    //
    //
    //__________________________________________________________________________
    class BufferedStreamCipher : public StreamCipher
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit BufferedStreamCipher() noexcept;  //!< setup

    public:
        virtual ~BufferedStreamCipher() noexcept;  //!< cleanup

        //______________________________________________________________________
        //
        //
        // [InputStream]
        //
        //______________________________________________________________________
        virtual bool query(char &C);
        virtual void store(const char C);
        virtual bool ready() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        size_t       onTap() const noexcept; //!< internal buffer size



    protected:
        IO::Chars buffer; //!< temporary output buffer

    private:
        Y_DISABLE_COPY_AND_ASSIGN(BufferedStreamCipher);
    };
}


#endif


