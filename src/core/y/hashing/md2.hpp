//! \file
#ifndef Y_HASHING_MD2_INCLUDED
#define Y_HASHING_MD2_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        /* RFC 1319 */
        namespace RFC1319
        {
            //! context for MD2
            struct MD2_CTX
            {
                uint8_t      state[16];    /*!< state                      */
                uint8_t      checksum[16]; /*!< checksum                   */
                size_t       count;        /*!< number of bytes, modulo 16 */
                uint8_t      buffer[16];   /*!< input buffer               */
            };
        }

        //! MD2
        class MD2 : public Function
        {
        public:
            Y_HASHING_FUNCTION_DECL(MD2,16,48); //!< specs

        private:
            RFC1319::MD2_CTX ctx;
            Y_DISABLE_COPY_AND_ASSIGN(MD2);
        };

    }
}

#endif
