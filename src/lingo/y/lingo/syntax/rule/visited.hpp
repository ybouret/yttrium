
//! \file

#ifndef Y_Lingo_Syntax_Rule_Visited_Included
#define Y_Lingo_Syntax_Rule_Visited_Included 1

#include "y/lingo/syntax/rule.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            

            class Visit  : public AddressBook
            {
            public:

                explicit Visit();
                virtual ~Visit() noexcept;

                void operator()(const Rule &);


            private:
                Y_DISABLE_ASSIGN(Visit);
            };
        }

    }

}

#endif

