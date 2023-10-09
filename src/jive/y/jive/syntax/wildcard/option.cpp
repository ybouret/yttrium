
#include "y/jive/syntax/wildcard/option.hpp"
#include "y/stream/xmlog.hpp"

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
            Wildcard( MakeOptionName(rr),UUID,rr)
            {
                I_am<Option>(this);
            }

            Option:: ~Option() noexcept
            {
            }

            const char * const Option:: CallSign = "Option";

            bool Option:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Y_XML_SECTION_OPT(xml,CallSign, " name='" << name << "'");
                XTree localTree = 0;
                if(rule.accepts(lexer,source,localTree,xml))
                {
                    Y_XMLOG(xml, "<found/>");
                    XNode::Expand(tree,localTree);
                }
                else
                {
                    Y_XMLOG(xml, "<!found/>");
                }
                return true;
            }

            void Option:: vizCore(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                fp << ",shape=diamond";
                fp << ']';
                Endl(fp);
            }

            bool Option:: isFrail() const noexcept
            {
                return true;
            }
        }

    }

}

