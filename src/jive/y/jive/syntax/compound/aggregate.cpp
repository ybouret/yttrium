
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
                for(const NodeType *sub=head;sub;sub=sub->next)
                {
                    const Rule &rule = **sub;
                    if( !rule.accepts(lexer, source, localTree) )
                    {
                        XNode::BackTo(lexer,localTree);
                        return false;
                    }
                }
                XNode::Expand(tree,localTree);
                return true;
            }

            Aggregate & Aggregate:: operator +=(const Rule &rr)
            {
                (*this) << rr;
                return *this;
            }

            void Aggregate:: vizCore(OutputStream &fp) const
            {
                node(fp) << '[';
                Label(fp,*name);
                fp << ']';
                Endl(fp);
            }


            bool Aggregate:: isFrail() const noexcept
            {
                if(size<=0)
                    return false;

                for(const NodeType *sub=head;sub;sub=sub->next)
                {
                    if( (**sub).isTough() ) return false;
                }
                return true;
            }
        }

    }

}

