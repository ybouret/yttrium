
//! \file

#ifndef Y_Base64_Decoder_Included
#define Y_Base64_Decoder_Included 1

#include "y/codec/buffered.hpp"


namespace Yttrium
{

    namespace Base64
    {

        class Decoder : public BufferedCodec
        {
        public:
            static const char * const CallSign;      //!< "Base64-Decoder"
            static const char         Padding = '='; //!< padding char

            explicit Decoder() noexcept; //!< setup
            virtual ~Decoder() noexcept; //!< cleanup


            // [Identifiable]
            virtual const char * callSign() const noexcept; //!< CallSign

            // [OutputStream]
            virtual void write(const char c); //!< append char to state, decode if possible
            virtual void flush();             //!< assume no padding

            // [StreamCipher]
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

