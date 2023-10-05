
//! \file

#ifndef Y_Jive_Syntax_Grammar_Included
#define Y_Jive_Syntax_Grammar_Included 1

#include "y/jive/syntax/rule.hpp"

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
                
                template <typename RULE> inline
                RULE & add(RULE *rule) {
                    assert(0!=rule);
                    add( static_cast<Rule *>(rule) );
                    return *rule;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                class Code;
                Code *code;

                static Code *Initialize(const Tag &);
                void         add(Rule *);
            };

        }

    }
}

#endif
