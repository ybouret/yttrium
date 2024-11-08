
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
                viz(fp);
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
        }

    }
}


