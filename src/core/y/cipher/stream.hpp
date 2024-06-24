
//! \file

#ifndef Y_StreamCipher_Included
#define Y_StreamCipher_Included 1

#include "y/cipher/interface.hpp"

namespace Yttrium
{

    class StreamCipher : public Cipher
    {
    protected:
        explicit StreamCipher() noexcept;

    public:
        virtual ~StreamCipher() noexcept;



    private:
        Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);
    };

}

#endif
