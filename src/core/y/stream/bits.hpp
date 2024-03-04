
//! \file

#ifndef Y_Stream_Bits_Included
#define Y_Stream_Bits_Included 1

#include "y/stream/io/char.hpp"
#include "y/stream/io/stock.hpp"

#include "y/data/list/cxx.hpp"
#include "y/config/shallow.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{

    namespace IO
    {
        Y_SHALLOW_DECL(_1); //!< alias
        Y_SHALLOW_DECL(_0); //!< alias
    };


    //__________________________________________________________________________
    //
    //
    //! Stream of Bits
    //
    //__________________________________________________________________________
    class StreamBits : public CxxListOf<IO::Char>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const uint8_t _1 = 0x01; //!< '1'
        static const uint8_t _0 = 0x00; //!< '0'

        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit StreamBits() noexcept; //!< setup empty
        virtual ~StreamBits() noexcept; //!< cleanup
        Y_OSTREAM_PROTO(StreamBits);    //!< display as string

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        StreamBits & operator<<( const IO::_1_ &); //!< push 1/true
        StreamBits & operator<<( const IO::_0_ &); //!< push 0/false


        //! push lower bits of data
        template <typename T>
        StreamBits & push(const T data,
                          size_t  bits)
        {
            static const T mask = 1;
            while(bits-- > 0)
                toTail( static_cast<uint8_t>( mask&(data>>bits)) );
            return *this;
        }

        //! pull lower bits of data
        template <typename T>
        T pull(size_t bits) noexcept
        {
            assert(size>=bits);
            assert(bits<=sizeof(T)*8);

            T ans(0);
            while(bits-- >0 )
            {
                IO::Char *ch = pool.store( popHead() );
                assert(_0==**ch||_1==**ch);
                ans <<= 1;
                ans |= static_cast<T>(**ch);
            }
            return ans;
        }

        //! push 0 to byte boundary
        StreamBits & fill();
        
        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________

        IO::Stock pool; //!< pool of chars/bits
    private:
        Y_DISABLE_COPY_AND_ASSIGN(StreamBits);
        IO::Char *build(const uint8_t);
        void      toTail(const uint8_t);
    };

}

#endif
