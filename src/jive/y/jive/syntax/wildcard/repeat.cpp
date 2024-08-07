#include "y/jive/syntax/wildcard/repeat.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            static
            String *MakeRepeatName(const Rule &rr, const unsigned nmin)
            {
                AutoPtr<String> res = new String( *(rr.name) );
                switch(nmin)
                {
                    case 0: (*res) += '*'; break;
                    case 1: (*res) += '+'; break;
                    default:
                        (*res) += Formatted::Get(">=%u",nmin);
                }
                return res.yield();
            }

            Repeat:: Repeat(const Rule &rr, const unsigned nmin) :
            Wildcard( MakeRepeatName(rr,nmin), UUID, rr),
            atLeast(nmin )
            {
                I_am<Repeat>(this);
            }

            Repeat:: ~Repeat() noexcept
            {
            }


            const char * const Repeat:: CallSign = "Repeat";

            bool Repeat:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Y_XML_SECTION_OPT(xml,CallSign, " name='" << name << "'");
                XTree    sub = XNode::Create(*this);
                unsigned num = 0;
                while( rule.accepts(lexer,source,sub,xml) )
                {
                    ++num;
                }
                Y_XMLOG(xml, "-- repeat=" << num << "/" << atLeast);
                if(num>=atLeast)
                {
                    Y_XMLOG(xml, "<success/>");
                    if(tree.isValid())
                    {
                        assert(tree->type==IsInternal);
                        tree->fusion(sub);
                    }
                    else
                    {
                        Y_XMLOG(xml, "-- badly designed grammar!");
                        tree = sub;
                    }
                    return true;
                }
                else
                {
                    Y_XMLOG(xml, "<failure/>");
                    XNode::BackTo(lexer,sub);
                    return false;
                }
            }

            void Repeat:: vizCore(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                fp << ",shape=octagon";
                fp << ']';
                Endl(fp);
            }

            bool Repeat:: isFrail() const noexcept
            {
                return atLeast <= 0;
            }
        }

    }

}

