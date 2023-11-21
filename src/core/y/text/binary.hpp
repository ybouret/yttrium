
//! \file

#ifndef Y_Text_Binary_Included
#define Y_Text_Binary_Included 1

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
    //! Binary utilities
    //
    //
    //__________________________________________________________________________
    class Binary
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const size_t MaxBits  = 64;                   //!< for uint64_t
        static const size_t Required = MaxBits+1;            //!< C-style text
        static const size_t DataSize = Y_MEMALIGN(Required); //!< sizeof(data)

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
        //
        // C++
        //
        //______________________________________________________________________

        //! format
        template <typename T> inline
        Binary(const T value, const OutputSize outputSize=Default) noexcept :
        data()
        {
            setup(static_cast<uint64_t>(value),
                  sizeof(T),
                  outputSize);
        }

        Binary(const Binary &other) noexcept; //!< copy
        ~Binary()                   noexcept; //!< cleanup

        Y_OSTREAM_PROTO(Binary);              //!< display
        Y_OUTPUT_STREAM(Binary);              //!< output

    private:
        Y_DISABLE_ASSIGN(Binary);
        char data[ DataSize ];
        void setup(const uint64_t   qw,
                   const size_t     nw,
                   const OutputSize outputSize) noexcept;

    };

}

#endif

