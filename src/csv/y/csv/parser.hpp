//! \file

#ifndef Y_CSV_Parser_Included
#define Y_CSV_Parser_Included 1

#include "y/jive/parser.hpp"

namespace Yttrium
{

    namespace CSV
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

