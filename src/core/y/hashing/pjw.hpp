//! \file
#ifndef Y_HASHING_PJW_INCLUDED
#define Y_HASHING_PJW_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{

        //! PJW
		class PJW : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(PJW,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(PJW);
		};
		
	}
}

#endif
