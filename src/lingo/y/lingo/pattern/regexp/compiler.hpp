
//! file

#ifndef Y_Lingo_RegExp_Compiler_Included
#define Y_Lingo_RegExp_Compiler_Included 1

#include "y/lingo/pattern/dictionary.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        class RXC
        {
        public:
            explicit RXC();
            virtual ~RXC() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(RXC);
        };
    }
}

#endif

