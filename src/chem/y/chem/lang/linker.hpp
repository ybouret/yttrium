
//! \file

#ifndef Y_Chemical_Lang_Linker_Included
#define Y_Chemical_Lang_Linker_Included 1

#include "y/jive/syntax/analyzer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Linker : public Jive::Syntax::Analyzer
        {
        public:
            explicit Linker() noexcept;
            virtual ~Linker() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
        };

    }

}

#endif

