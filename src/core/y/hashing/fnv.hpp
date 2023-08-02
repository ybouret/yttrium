//! \file
#ifndef Y_HASHING_FNV_INCLUDED
#define Y_HASHING_FNV_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{

        //! FNV
		class FNV : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(FNV,4,4);
            
            
		private:
			uint32_t hash;
            Y_DISABLE_COPY_AND_ASSIGN(FNV);
		};
		
	}
}

#endif
