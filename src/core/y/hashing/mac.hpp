
//! \file

#ifndef Y_HASHING_MAC_INCLUDED
#define Y_HASHING_MAC_INCLUDED 1


#include "y/hashing/hmac.hpp"
#include "y/ptr/light-string.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        //! helper for constructor
#define YACK_HMAC_ON_PROLOG() \
Function( FUNCTION::__length, FUNCTION::__window), H(), M(H,

        //! helper for constructor
#define YACK_HMAC_ON_EPILOG() \
), id(HashMAC::Prefix,H.callSign())
        
        //______________________________________________________________________
        //
        //
        //! assembling HMAC function
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class HMAC : public Function
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit HMAC(const void *key_addr, const size_t key_size) :
            YACK_HMAC_ON_PROLOG() key_addr,key_size YACK_HMAC_ON_EPILOG() {}

            //! setup
            inline explicit HMAC(const Memory::ReadOnlyBuffer &usr) :
            YACK_HMAC_ON_PROLOG() usr  YACK_HMAC_ON_EPILOG() {}

            //! setup
            inline explicit HMAC(const char *k) :
            YACK_HMAC_ON_PROLOG() k,StringLength(k) YACK_HMAC_ON_EPILOG() {}

            //! cleanup
            inline virtual ~HMAC() noexcept {}

            //__________________________________________________________________
            //
            // function interface
            //__________________________________________________________________

            //! prepare function
            inline virtual void set() noexcept { M.set(H); }

            //! run function
            inline virtual void run(const void *block_addr, size_t block_size) noexcept
            {
                H.run(block_addr,block_size);
            }

            //! get function from hmac
            inline virtual void get(void *output, const size_t outlen) noexcept
            {
                const Memory::ReadOnlyBuffer &md = M.get(H);
                fill(output,outlen,md.ro_addr(),md.measure());
            }

            //! compound name
            virtual const char *callSign() const noexcept { return id(); }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            FUNCTION H; //!< base hashing function
            HashMAC  M; //!< base hash mac creator
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(HMAC);
            const LightString id;
        };

    }

}

#endif
