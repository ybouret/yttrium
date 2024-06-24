
//! \file

#ifndef Y_StreamCipher_Included
#define Y_StreamCipher_Included 1

#include "y/cipher/interface.hpp"
#include "y/stream/input.hpp"
#include "y/stream/output.hpp"

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
    class StreamCipher : public Cipher, public InputStream, public OutputStream
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

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual void reset() noexcept = 0; //!< reset all internal state


    private:
        Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);
    };

}

#endif
