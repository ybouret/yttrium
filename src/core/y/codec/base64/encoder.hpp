
//! \file

#ifndef Y_Base64_Encoder_Included
#define Y_Base64_Encoder_Included 1

#include "y/codec/buffered.hpp"


namespace Yttrium
{

    namespace Base64
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base64 Encoder
        //
        //
        //______________________________________________________________________
        class Encoder : public BufferedCodec
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Base64-Encoder"

            //! internal state
            enum State
            {
                WaitFor1, //!< wait for 1st char in input[0]
                WaitFor2, //!< wait for 2nd char in input[1]
                WaitFor3  //!< wait for 3rd char in input[2]
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Encoder(bool padding) noexcept; //!< setup
            virtual ~Encoder()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // [Identifiable]
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept;

            //__________________________________________________________________
            //
            //
            // [OutputStream]
            //
            //__________________________________________________________________
            virtual void write(const char C);
            virtual void flush();

            //__________________________________________________________________
            //
            //
            // [Cipher]
            //
            //__________________________________________________________________
            virtual void reset() noexcept;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            bool   doPad; //!< padding option

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            State  state;
            char   input[4];
            const char * table;

            void waitFor1() noexcept;
            void flush1();
            void flush2();
            void flush3();
            void send(const char *p, size_t n);

        };

    }

}

#endif

