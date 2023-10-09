
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
                Node(fp,this) << '[';
                Label(fp,*(rule.name));
                fp << ']';
                Endl(fp);

                switch(type)
                {
                    case IsTerminal: vizTerminal(fp); break;
                    case IsInternal: vizInternal(fp); break;
                }
            }

            void XNode:: vizTerminal(OutputStream &fp) const
            {

            }

            void XNode :: vizInternal(OutputStream &fp) const
            {
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
