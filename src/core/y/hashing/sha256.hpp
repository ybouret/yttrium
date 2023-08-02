//! \file
#ifndef Y_HASHING_SHA256_INCLUDED
#define Y_HASHING_SHA256_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{
        //! context for sha256/224
		typedef struct 
		{
			uint32_t      total[2];     /*!< number of bytes processed  */
			uint32_t      state[8];     /*!< intermediate digest state  */
			uint8_t       buffer[64];   /*!< data block being processed */
			int           is224;        /*!< false => SHA-256, else SHA-224 */
		} SHA256_CTX;
		

        //! SHA-256
		class SHA256 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(SHA256,32,64); //!< specs

            static void Tests(); //!< tests
            
		private:
			SHA256_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(SHA256);
		};

        //! SHA-224
		class SHA224 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(SHA224,28,64); //!< specs

            static void Tests(); //!< tests

		private:
			SHA256_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(SHA224);
		};
		
		
		
	}
}

#endif


