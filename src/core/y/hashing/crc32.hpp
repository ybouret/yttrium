//! \file
#ifndef Y_HASHING_CRC32_INCLUDED
#define Y_HASHING_CRC32_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{
        //! crc32
		class CRC32 : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(CRC32,4,4);
            
            
		private:
			uint32_t    hash;
			Y_DISABLE_COPY_AND_ASSIGN(CRC32);
		};
		
	}
}

#endif
