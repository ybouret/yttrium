//! \file
#ifndef Y_HASHING_SHA512_INCLUDED
#define Y_HASHING_SHA512_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{
        //! context for sha384/sha512
		struct SHA512_CTX
		{
			uint64_t total[2];          /*!< number of bytes processed  */
			uint64_t state[8];          /*!< intermediate digest state  */
			uint8_t  buffer[128];  		/*!< data block being processed */
			int      is384;             /*!< false => SHA-512, else SHA-384 */
		};
		

        //! SHA-512
		class SHA512 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(SHA512,64,128); //!< specs
			
		private:
			SHA512_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(SHA512);
		};

        //! SHA-384
		class SHA384 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(SHA384,48,128); //!< specs
			
		private:
			SHA512_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(SHA384);
		};
		
	}
	
}

#endif

	
	
