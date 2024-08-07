
//! \file

#ifndef Y_Codec_Included
#define Y_Codec_Included 1

#include "y/type/identifiable.hpp"
#include "y/stream/input.hpp"
#include "y/stream/output.hpp"
#include <cstring>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! base class for Codecs
    //
    //
    //__________________________________________________________________________
    class Codec : public InputStream, public OutputStream
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Codec() noexcept;
        
    public:
        virtual ~Codec() noexcept;

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual void reset() noexcept = 0; //!< reset all internal state


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! send all internal chars to output via operator<<
        template <typename OUTPUT> inline
        void sendTo(OUTPUT &output) { char C=0; while(query(C)) output << C; }

        //! flush and sendTo(output)
        template <typename OUTPUT> inline
        void flushIn(OUTPUT &output) { flush(); sendTo(output); }


        //! process msg[len] and send to output, flushing if requested
        template <typename OUTPUT> inline
        void operator()(OUTPUT &output, const char * msg,size_t len, const bool wouldFlush)
        {
            OutputStream &target = *this;
            while( len-- > 0 ) {
                target << *(msg++);
                sendTo(output);
            }
            if(wouldFlush) flushIn(output);
        }

        //! process binary input, flushing if requested
        template <typename OUTPUT> inline
        void operator()(OUTPUT &output, const Memory::ReadOnlyBuffer &input, const bool wouldFlush)
        {
            Codec &sc = *this;
            sc(output, static_cast<const char *>(input.ro_addr()), input.measure(), wouldFlush);
        }


        //! process text, flushing if requested
        template <typename OUTPUT> inline
        void operator()(OUTPUT &output, const char * const msg, const bool wouldFlush)
        {
            Codec &sc = *this;
            if(0!=msg)
                sc(output, msg, strlen(msg), wouldFlush);
            else
                sc(output, msg, 0, wouldFlush);
        }

        //! process all input, flushing after EOF if requested
        template <typename OUTPUT> inline
        void operator()(OUTPUT &output, InputStream &input, bool wouldFlush)
        {
            OutputStream &target = *this;
            char          C      = 0;
            while(input.query(C))
            {
                target << C;
                sendTo(output);
            }
            if(wouldFlush) flushIn(output);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Codec);
    };

}

#endif

