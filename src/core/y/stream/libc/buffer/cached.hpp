

//! \file

#ifndef Y_IO_Libc_Cached_Buffer_Included
#define Y_IO_Libc_Cached_Buffer_Included 1

#include "y/stream/libc/buffer/system.hpp"
#include "y/stream/io/stock.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    namespace Libc
    {

        //______________________________________________________________________
        //
        //
        //
        //! buffer with cached access for input file
        //
        //
        //______________________________________________________________________
        class CachedBuffer : public SystemBuffer, public IO::Chars
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit CachedBuffer();           //!< setup
            virtual ~CachedBuffer() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     ready();           //!< stock.size>=bytes
            void     prune() noexcept;  //!< size+stock.size == bytes
            void     unget(const char); //!< force [new|from stock] char at head

            //! with stock.size>=upper+1-lower, load byte[lower,upper] into Chars
            void     bring(const size_t lower, const size_t upper) noexcept;

            //! extract head, size>0
            char     pluck() noexcept;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            IO::Stock stock; //!< local memory

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CachedBuffer);
        };
    }

}


#endif
