//! \file

#ifndef Y_IO_Chars_Included
#define Y_IO_Chars_Included 1

#include "y/stream/io/char.hpp"
#include "y/data/list/cxx.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace IO
    {
        //______________________________________________________________________
        //
        //
        //
        //! Versatile list of Chars
        //
        //
        //______________________________________________________________________
        class Chars : public CxxListOf<Char>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Chars() noexcept; //!< setup empty
            virtual ~Chars() noexcept; //!< cleanup
            Chars(const Chars &);      //!< copy
            Y_OSTREAM_PROTO(Chars);    //!< display using memory block
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Chars & operator<<(const uint8_t); //!< pushTail
            Chars & operator>>(const uint8_t); //!< pushHead
            Chars & operator<<(const char *);  //!< pushTail
            Chars & operator>>(const char *);  //!< pushHead

            uint8_t pullTail() noexcept; //!< extract and remove tail
            uint8_t pullHead() noexcept; //!< extract and remove head
            


            
        private:
            Y_DISABLE_ASSIGN(Chars);
        };
    }

}

#endif

