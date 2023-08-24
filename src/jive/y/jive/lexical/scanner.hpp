//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Scanner
        {
        public:
            template <typename NAME>
            explicit Scanner(NAME name) : mark(name) {}
            virtual ~Scanner() noexcept;
            
            
            const Mark mark;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scanner);
        };

    }
}

#endif
