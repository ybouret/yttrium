#include "y/jive/syntax/compound/alternate.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Alternate:: ~Alternate() noexcept
            {
            }

            Alternate & Alternate:: operator |=(const Rule &rr)
            {
                (*this) << rr;
                return *this;
            }

            bool Alternate:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                XTree subTree = 0;
                bool  success = false;
                for(const NodeType *node=head;node;node=node->next)
                {
                    const Rule &subRule = **node;
                    if( subRule.accepts(lexer,source,subTree) )
                    {
                        success = true;
                        if(subTree.isValid())
                        {
                            XNode::Expand(tree,subTree.yield());
                            return true;
                        }
                        // else it was a feable rule: success but try again
                    }
                }
                
                // a feable rule matched
                return success;
            }


            void Alternate:: vizCore(OutputStream &fp) const
            {
                node(fp) << '[';
                Label(fp,*name);
                fp << ']';
                Endl(fp);
            }
        }

    }

}

