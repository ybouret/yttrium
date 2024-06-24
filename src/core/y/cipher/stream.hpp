
//! \file

#ifndef Y_StreamCipher_Included
#define Y_StreamCipher_Included 1

#include "y/cipher/interface.hpp"
#include "y/stream/input.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{

    class StreamCipher : public Cipher, public InputStream, public OutputStream
    {
    protected:
        explicit StreamCipher() noexcept;

    public:
        virtual ~StreamCipher() noexcept;

        virtual void reset() noexcept = 0;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);
    };

}

#endif
