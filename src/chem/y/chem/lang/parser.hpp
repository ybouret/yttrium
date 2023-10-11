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
            typedef Jive::Syntax::XNode XNode;

            explicit Parser();
            virtual ~Parser() noexcept;

            const Jive::Tag reac;
            const Jive::Tag prod;
            const Jive::Tag plus;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            XNode *prune(XNode *) noexcept;
            void scrub(XNode *)   noexcept;
        };

    }
}

#endif

