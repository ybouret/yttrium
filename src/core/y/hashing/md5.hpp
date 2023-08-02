//! \file
#ifndef Y_HASHING_MD5_INCLUDED
#define Y_HASHING_MD5_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{
		/* RFC 1321 */
		namespace RFC1321
		{
            //! context to compute MD5
			struct MD5_CTX
			{
				uint32_t      state[4];        /*!< state (ABCD) */
				uint32_t      count[2];        /*!< number of bits, modulo 2^64 (lsb first) */
				uint8_t       buffer[64];      /*!< input buffer */		
			};
		}

        //! MD5
		class MD5 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(MD5,16,64); //!< specs
            static void Tests();                //!< tests from RFC
            
		private:
			RFC1321::MD5_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(MD5);
		};

	}
}

#endif
