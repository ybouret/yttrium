
#include "y/jive/syntax/compound/aggregate.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Aggregate:: ~Aggregate() noexcept
            {
            }

            const char * const Aggregate:: CallSign = "Aggregate";

            bool Aggregate:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Y_XML_SECTION_OPT(xml,CallSign, " name='" << name << "' size='" << size << "'");
                XTree localTree = XNode::Create(*this);
                for(const NodeType *sub=head;sub;sub=sub->next)
                {
                    const Rule &rule = **sub;
                    if( !rule.accepts(lexer,source,localTree,xml) )
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

            Aggregate & Aggregate:: operator <<(const Rule &rr)
            {
                Manifest &self = *this;
                self << rr;
                return *this;
            }

            void Aggregate:: vizCore(OutputStream &fp) const
            {
                node(fp) << '[';
                Label(fp,*name);
                fp << ",shape=house";
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

