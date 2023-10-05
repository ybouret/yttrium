
//! \file

#ifndef Y_Jive_Syntax_Rule_Included
#define Y_Jive_Syntax_Rule_Included 1

#include "y/jive/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            class Grammar : public Entity
            {
            public:
                virtual ~Grammar() noexcept;

                template <typename ID> inline
                explicit Grammar(const ID &id) :
                Entity(id), code( Initialize(name) )
                {}
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                class Code;
                Code *code;

                static Code *Initialize(const Tag &);
            };

        }

    }
}

#endif
