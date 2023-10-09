#include "y/jive/syntax/compound/alternate.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Alternate:: ~Alternate() noexcept
            {
            }


            Alternate & Alternate:: operator <<(const Rule &rr)
            {
                Manifest &self = *this;
                self << rr;
                return *this;
            }

            Alternate & Alternate:: operator |=(const Rule &rr)
            {
                (*this) << rr;
                return *this;
            }

            const char * const Alternate::CallSign = "Alternate";

            bool Alternate:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Y_XML_SECTION_OPT(xml,CallSign, " name='" << name << "' size='" << size << "'");
                XTree subTree = 0;
                bool  success = false;
                for(const NodeType *sub=head;sub;sub=sub->next)
                {
                    const Rule &subRule = **sub;
                    if( subRule.accepts(lexer,source,subTree,xml) )
                    {
                        Y_XMLOG(xml, "<found[" << subRule.name << "]/>");
                        success = true;
                        if(subTree.isValid())
                        {
                            XNode::Expand(tree,subTree.yield());
                            return true;
                        }
                        // else it was a frail rule: success but try again
                    }
                }
                
                // a frail rule matched, or empty
                Y_XMLOG(xml, (success ? "<success/>" : "<failure/>") );
                return success;
            }


            void Alternate:: vizCore(OutputStream &fp) const
            {
                node(fp) << '[';
                Label(fp,*name);
                fp << ", shape=egg";
                fp << ']';
                Endl(fp);
            }

            bool Alternate:: isFrail() const noexcept
            {
                for(const NodeType *sub=head;sub;sub=sub->next)
                {
                    if( (**sub).isFrail() ) return true;
                }
                return false;
            }

        }

    }

}

