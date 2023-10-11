//! \file

#ifndef Y_Chemical_Lang_Parser_Included
#define Y_Chemical_Lang_Parser_Included 1

#include "y/jive/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Parser : public Jive::Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() noexcept;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }
}

#endif

