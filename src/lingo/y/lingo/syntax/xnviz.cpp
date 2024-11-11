
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

            void XNode:: vizTerminal(OutputStream &fp) const
            {
                Node(fp,this) << '[';

                const Syntax::Terminal &term = *rule.as<Syntax::Terminal>();

                // prepare label
                {
                    fp << "label=\"";
                    Text(fp, name() );
                    switch(term.kind)
                    {
                        case Syntax::Terminal::Univocal: break;
                        case Syntax::Terminal::Standard: {
                            const String &info = lexeme().toString();
                            Text(fp << "='",info) << "'";
                        } break;
                    }
                    fp << "\"";
                }

                // modify shape
                rule.emitShape(fp);

                // modify style
                fp << ",style=\"filled";

                fp << "\"";
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
                rule.emitShape(fp);

                // modify according to UUID
                switch(rule.uuid)
                {
                    default:
                        throw Specific::Exception("XNode::vizInternal","unexpected internal '%s'", name().c_str());
                }

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
