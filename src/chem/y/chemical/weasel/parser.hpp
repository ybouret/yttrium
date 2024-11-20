
//! \file

#ifndef Y_Chemical_Weasel_Parser_Included
#define Y_Chemical_Weasel_Parser_Included 1

#include "y/chemical/species.hpp"
#include "y/lingo/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {
            class Parser : public Lingo::Parser
            {
            public:
                explicit Parser();
                virtual ~Parser() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };
        }
    }

}

#endif


