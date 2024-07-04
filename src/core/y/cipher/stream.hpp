
//! \file

#ifndef Y_StreamCipher_Included
#define Y_StreamCipher_Included 1

#include "y/cipher/interface.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Stream Cipher: write input data, query output transformed
    //
    //
    //__________________________________________________________________________
    class StreamCipher : public Cipher
    {
        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit StreamCipher() noexcept; //!< setup

    public:
        virtual ~StreamCipher() noexcept; //!< cleanup

        virtual uint8_t call() noexcept = 0;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);

    };

}

#endif
