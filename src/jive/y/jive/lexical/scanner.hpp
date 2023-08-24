//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            enum Action
            {
                Produce,
                Control
            };

            typedef Functor<Action,TL1(const Token&)> CallBack;

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
}

#endif
