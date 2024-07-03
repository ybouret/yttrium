
//! \file

#ifndef Y_Base64_Decoder_Included
#define Y_Base64_Decoder_Included 1

#include "y/codec/buffered.hpp"


namespace Yttrium
{

    namespace Base64
    {

        //______________________________________________________________________
        //
        //
        //
        //! Base64 Decoder
        //
        //
        //______________________________________________________________________
        class Decoder : public BufferedCodec
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign;      //!< "Base64-Decoder"
            static const char         Padding = '='; //!< padding char

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Decoder() noexcept; //!< setup
            virtual ~Decoder() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // [Identifiable]
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< CallSign

            //__________________________________________________________________
            //
            //
            // [OutputStream]
            //
            //__________________________________________________________________
            virtual void write(const char c); //!< append char to state, decode if possible
            virtual void flush();             //!< assume no padding

            //__________________________________________________________________
            //
            //
            // [Codec]
            //
            //__________________________________________________________________
            virtual void reset() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Decoder);
            unsigned state;
            char     input[4];

            void reset1() noexcept;

        };

    }

}

#endif

