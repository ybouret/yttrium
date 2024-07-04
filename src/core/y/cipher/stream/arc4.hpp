
//! \file

#ifndef Y_ARC4_Included
#define Y_ARC4_Included 1

#include "y/cipher/stream.hpp"

namespace Yttrium
{
    namespace Crypto
    {
        //! Alledged RC4
        class ARC4 : public StreamCipher
        {
        public:
            static const char * const CallSign; //!< "ARC4"

            //! setup
            explicit ARC4( const Memory::ReadOnlyBuffer &key ) noexcept;
            
            //! cleanup
            virtual ~ARC4() noexcept;
            
            virtual const char * callSign() const noexcept;

            //! reset with a given key
            virtual void schedule(const Key &key) noexcept;
            
            //! next byte
            virtual uint8_t  call() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ARC4);
            unsigned x;
            unsigned y;
            uint8_t  S[256];
            void    init( const uint8_t *key, size_t len) noexcept;;
        };
    }
}


#endif

