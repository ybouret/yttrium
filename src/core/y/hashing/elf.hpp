//! \file
#ifndef Y_HASHING_ELF_INCLUDED
#define Y_HASHING_ELF_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{

        //! ELF
		class ELF : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(ELF,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(ELF);
		};
		
	}
}

#endif
