
#include "y/lingo/syntax/rules.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            const String & XNode:: name() const noexcept
            {
                return *rule.name;
            }

            void XNode:: viz(OutputStream &fp) const
            {
                switch(type)
                {
                    case Terminal: vizTerminal(fp); break;
                    case Internal: vizInternal(fp); break;
                }

            }

            void XNode:: graphViz(OutputStream &fp)  const
            {
                Enter(fp,"G");
                viz(fp);
                Leave(fp);
            }

            void XNode:: vizTerminal(OutputStream &fp) const
            {
                Node(fp,this) << '[';

                const Syntax::Terminal &term = *rule.as<Syntax::Terminal>();

                // prepare label
                {
                    fp << "label=\"";
                    Text(fp, name() );
                    {
                        const String &info = lexeme().toString();
                        if(info.size()>0)
                            Text(fp << "='",info) << "'";
                    }
                    fp << "\"";
                }

                term.vizMark(fp);

                Endl(fp << ']');
            }

            void XNode:: vizInternal(OutputStream &fp) const
            {
                // emit list
                for(const XNode *node=list().head;node;node=node->next)
                {
                    node->viz(fp);
                }

                // emit this
                Node(fp,this) << '[';
                Label(fp,name());
                rule.vizMark(fp);
                Endl(fp<<']');

                // link to list with numbering if necessary
                switch(list().size)
                {
                    case 0: return;
                    case 1: Endl(Arrow(fp,this,list().head)); return;
                    default:
                        break;
                }

                unsigned i = 1;
                for(const XNode *node=list().head;node;node=node->next,++i)
                {
                    Arrow(fp,this,node) << '[';
                    const String _ = Formatted::Get("%u",i);
                    Label(fp,_);
                    Endl(fp<<']');
                }


            }



        }

    }

}
