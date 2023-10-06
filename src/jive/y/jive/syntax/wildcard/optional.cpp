
#include "y/jive/syntax/wildcard/optional.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            static inline
            String * MakeOptionalName(const Rule &rr)
            {
                AutoPtr<String> res = new String(*(rr.name));
                (*res) += '?';
                return res.yield();
            }

            Optional:: Optional(const Rule &rr) :
            Wildcard( MakeOptionalName(rr),UUID,rr) {}

            Optional:: ~Optional() noexcept
            {
            }


            bool Optional:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                XTree localTree = 0;
                if(rule.accepts(lexer,source,localTree))
                {
                    XNode::Expand(tree,localTree);
                }
                return true;
            }
        }

    }

}

