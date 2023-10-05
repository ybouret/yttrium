
#include "y/jive/syntax/compound/aggregate.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Aggregate:: ~Aggregate() noexcept
            {
            }

            bool Aggregate:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {

                XTree localTree = XNode::Create(*this);
                for(const NodeType *node=head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    if( !rule.accepts(lexer, source, localTree) )
                    {
                        XNode::BackTo(lexer,localTree);
                        return false;
                    }
                }
                XNode::Expand(tree,localTree);
                return true;
            }
        }

    }

}

