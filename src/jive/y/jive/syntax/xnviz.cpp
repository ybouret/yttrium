
#include "y/jive/syntax/terminal.hpp"


namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            void XNode:: graphViz(OutputStream &fp) const
            {
                Enter(fp,"XNode");
                viz(fp);
                Leave(fp);
            }

            void XNode:: viz(OutputStream &fp) const
            {

                switch(type)
                {
                    case IsTerminal: vizTerminal(fp); break;
                    case IsInternal: vizInternal(fp); break;
                }
            }

            void XNode:: vizTerminal(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                if(Terminal::UUID==rule.uuid)
                {
                    const Terminal &t = *rule.as<Terminal>();
                    if(t.univocal)
                    {
                        Label(fp,*(rule.name));
                    }
                    else
                    {
                        const String title = *(rule.name) + '=' + '\'' + lptr->toString() + '\'';
                        Label(fp,title);
                    }

                    fp << ", shape=box";
                    
                }
                fp << ']';
                Endl(fp);
            }

            void XNode :: vizInternal(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*(rule.name));
                fp << ']';
                Endl(fp);

                unsigned   idx  = 1;
                const bool show = chld.size>1;
                for(const XNode *sub = chld.head; sub; sub=sub->next, ++idx)
                {
                    sub->viz(fp);
                    Arrow(fp,this,sub);
                    if(show)
                    {
                        fp("[label=\"%u\"]",idx);
                    }
                    Endl(fp);
                }
            }

        }



    }

}
