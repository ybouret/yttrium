
//! \file

#ifndef Y_Base64_Encode_Included
#define Y_Base64_Encode_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{

    namespace Base64
    {
        //______________________________________________________________________
        //
        //
        //! Base64 Encode operations
        //
        //______________________________________________________________________
        struct Encode
        {
            static const char Table[64];       //!< encoding table
            static const char Padding = '=';   //!< padding char

            template <size_t> struct OutputLengthFor;
            
            //! show internal data, mostly to debug
            static void   ShowInfo();

            //! 1 code => 2 chars, 4 chars with padding
            static size_t _1(char * output, const uint8_t code, const bool pad) noexcept;

            //! 2 code2 => 3 chars, 4 chars with padding
            static size_t _2(char * ouput, const uint8_t c0, const uint8_t c1, const bool pad) noexcept;

            //! 3 codes => 4 chaers
            static size_t _3(char *ouput, const uint8_t c0, const uint8_t c1, const uint8_t c2) noexcept;

            //! output size for input Size
            static size_t LengthFor(const size_t inputSize, const bool pad) noexcept;

            //! encode buffer length in output with size >= LengthFor(length,pad)
            static size_t To(char * output, const void *buffer, const size_t length, const bool pad) noexcept;
            

        };

        //! raw char buffer length for 1 byte
        template <> struct Encode::OutputLengthFor<1> { enum { Value= 4 }; };

        //! raw char buffer length for 2 bytes
        template <> struct Encode::OutputLengthFor<2> { enum { Value= 4 }; };

        //! raw char buffer length for 4 bytes
        template <> struct Encode::OutputLengthFor<4> { enum { Value= 8 }; };

        //! raw char buffer length for 8 bytes
        template <> struct Encode::OutputLengthFor<8> { enum { Value=12 }; };

    }

}


#endif
