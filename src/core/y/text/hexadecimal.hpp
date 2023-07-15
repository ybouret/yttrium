//! \file

#ifndef Y_Text_Hexadecimal_Included
#define Y_Text_Hexadecimal_Included 1

#include "y/type/ints.hpp"
#include "y/calculus/align.hpp"
#include "y/ostream-proto.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Hexadecimal utilities
    //
    //
    //__________________________________________________________________________
    class Hexadecimal
    {
    public:
        enum OutputSize
        {
            Default,
            Compact
        };

        enum OutputCase
        {
            LowerCase,
            UpperCase
        };

        static const char   Text[256][4];                   //!< "00".."ff"
        static const char   Lower[16];                      //!< 0..9a..f
        static const char   Upper[16];                      //!< 0..9A..F
        static int          ToDecimal(const char) noexcept; //!< -1 if invalid

        static const size_t CharsPerByte = 2;
        static const size_t WordBytes    = sizeof(uint64_t);
        static const size_t CharsPerWord = CharsPerByte * WordBytes;
        static const size_t Required     = CharsPerWord+1;
        static const size_t DataSize     = Y_MEMALIGN(Required);

        template <typename T>
        Hexadecimal(const T &value,
                    const OutputSize outputSize = Default,
                    const OutputCase outputCase = LowerCase) noexcept :
        data()
        {
            setup(static_cast<uint64_t>(value),
                  sizeof(value),
                  outputSize,
                  outputCase);
        }

        ~Hexadecimal() noexcept;
        Hexadecimal(const Hexadecimal &) noexcept;

        Y_OSTREAM_PROTO(Hexadecimal);

    private:
        Y_DISABLE_ASSIGN(Hexadecimal);
        char data[ DataSize ];
        void setup(const uint64_t   qw,
                   const unsigned   sz,
                   const OutputSize outputSize,
                   const OutputCase outputCase) noexcept;


    };


}

#endif

