
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
                if(this->sire)
                {
                    Arrow(fp, this, this->sire);
                    fp << "[color=red]";
                    Endl(fp);
                }
            }

            void XNode:: vizTerminal(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                if(rule.is<Terminal>())
                {
                    const Terminal &t = *rule.as<Terminal>();
                    if(t.univocal)
                    {
                        Label(fp,*(rule.name));
                        switch(t.property)
                        {
                            case Terminal::IsRegular:
                                fp << ",style=\"rounded,filled\"";
                                break;

                            case Terminal::IsDivider:
                                fp << ",style=\"rounded,dotted\"";
                                break;
                        }
                    }
                    else
                    {
                        const String title = *(rule.name) + '=' + '\'' + lptr->toString() + '\'';
                        Label(fp,title);
                        switch(t.property)
                        {
                            case Terminal::IsRegular:
                                fp << ",style=\"filled\"";
                                break;

                            case Terminal::IsDivider:
                                fp << ",style=\"dotted\"";
                                break;
                        }
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
                fp << ", shape=box";
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
