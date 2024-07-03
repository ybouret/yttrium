

//! \file

#ifndef Y_BufferedCodec_Included
#define Y_BufferedCodec_Included 1

#include "y/codec/interface.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //!  Codec an internal IO::Chars buffer
    //
    //
    //__________________________________________________________________________
    class BufferedCodec : public  Codec
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit BufferedCodec() noexcept;  //!< setup

    public:
        virtual ~BufferedCodec() noexcept;  //!< cleanup

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
        size_t       buffered() const noexcept; //!< internal buffer size



    protected:
        IO::Chars buffer; //!< temporary output buffer
        IO::Chars supply; //!< temporary supply
        
        void clearBuffer() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(BufferedCodec);
    };
}


#endif


