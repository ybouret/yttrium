
#include "y/jive/syntax/wildcard/option.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            static inline
            String * MakeOptionName(const Rule &rr)
            {
                AutoPtr<String> res = new String(*(rr.name));
                (*res) += '?';
                return res.yield();
            }

            Option:: Option(const Rule &rr) :
            Wildcard( MakeOptionName(rr),UUID,rr) {}

            Option:: ~Option() noexcept
            {
            }


            bool Option:: accepts(Y_JIVE_SYNTAX_RULE_API) const
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

