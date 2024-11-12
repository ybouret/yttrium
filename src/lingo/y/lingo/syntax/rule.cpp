
#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Rule:: ~Rule() noexcept
            {
            }


            void Rule:: graphViz(OutputStream &fp) const
            {
                Enter(fp, "G");
                vizCode(fp);
                Leave(fp);
            }

            bool Rule:: flimsy() const
            {
                return !robust();
            }


            size_t XNode:: serialize(OutputStream &fp) const
            {
                size_t ans = rule.name->serialize(fp);
                switch(type)
                {
                    case Terminal:
                        // write lexeme content
                        ans += fp.emitVBR(lexeme().size);
                        for(const Char *ch = lexeme().head; ch; ch=ch->next)
                        {
                            fp.write( **ch );
                            ++ans;
                        }
                        break;

                    case Internal:
                        // write list
                        ans += fp.emitVBR( list().size );
                        for(const XNode *node=list().head;node;node=node->next)
                        {
                            ans += node->serialize(fp);
                        }
                        break;
                }

                return ans;
            }

            void Rule:: vizCode(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                vizMark(fp);
                Endl(fp << ']');
                vizLink(fp);
            }

            void Rule:: vizLink(OutputStream &) const
            {

            }

            void Rule:: vizArrow(OutputStream &fp, const Rule &r) const
            {
                Endl(Arrow(fp,this,&r));
            }

        }

    }
}


