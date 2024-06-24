
//! \file

#ifndef Y_BufferedStreamCipher_Included
#define Y_BufferedStreamCipher_Included 1

#include "y/cipher/stream.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    class BufferedStreamCipher : public StreamCipher
    {
    protected:
        explicit BufferedStreamCipher() noexcept;

    public:
        virtual ~BufferedStreamCipher() noexcept;


        virtual bool query(char &C);
        virtual void store(const char C);
        virtual bool ready() noexcept;




    protected:
        IO::Chars buffer;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(BufferedStreamCipher);
    };
}


#endif


