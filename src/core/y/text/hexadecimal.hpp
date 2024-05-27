//! \file

#ifndef Y_Text_Hexadecimal_Included
#define Y_Text_Hexadecimal_Included 1

#include "y/type/ints.hpp"
#include "y/calculus/align.hpp"
#include "y/ostream-proto.hpp"
#include "y/output-stream.hpp"


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
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! formatting style
        //______________________________________________________________________
        enum OutputSize
        {
            Default, //!< full size
            Compact  //!< compact size
        };

        //______________________________________________________________________
        //
        //! formatting case
        //______________________________________________________________________
        enum OutputCase
        {
            LowerCase, //!< using Lower
            UpperCase  //!< using Upper
        };

        //______________________________________________________________________
        //
        // conversion helpers
        //______________________________________________________________________
        static const char   Text[256][4];                   //!< "00".."ff"
        static const char   Lower[16];                      //!< 0..9a..f
        static const char   Upper[16];                      //!< 0..9A..F
        static int          ToDecimal(const char) noexcept; //!< -1 if invalid

        //______________________________________________________________________
        //
        // metrics
        //______________________________________________________________________
        static const size_t CharsPerByte = 2;                        //!< two chars per byte
        static const size_t WordBytes    = sizeof(uint64_t);         //!< alias
        static const size_t CharsPerWord = CharsPerByte * WordBytes; //!< minimal text
        static const size_t Required     = CharsPerWord+1;           //!< C-style text
        static const size_t DataSize     = Y_MEMALIGN(Required);     //!< sizeof(data)


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup hexadecimal representation
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


        Hexadecimal(const Hexadecimal &) noexcept;         //!< copy data
        ~Hexadecimal() noexcept;                           //!< cleanup
        Y_OSTREAM_PROTO(Hexadecimal);                      //!< display
        Y_OUTPUT_STREAM(Hexadecimal);                      //!< output

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        //! make from address
        static Hexadecimal Address(const void *) noexcept;
        
        //! helper to display data
        template <typename T> static inline
        Hexadecimal From(const T &value) noexcept { return Hexadecimal(value); }


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

