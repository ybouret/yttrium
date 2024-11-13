#include "y/lingo/syntax/internal/compound/alternate.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() noexcept
            {
            }

            Alternate & Alternate:: operator |= (const Rule &rule)
            {
                *this << rule;
                return *this;
            }

            void Alternate:: vizMark(OutputStream &fp) const
            {
                fp << ",shape=egg";
            }

            void Alternate:: acknowledge() noexcept
            {
                manifest.noMultiple();
            }

            bool Alternate:: robust() const
            {
                checkNotEmpty("Alternate::robust()",name->c_str());
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    if( (**node).flimsy() ) return false;
                }
                return false;
            }

            bool Alternate:: accepts(Y_Lingo_Syntax_Args) const
            {
                assert(manifest.size>0);

                bool found = false;
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    XNode      *temp = 0;
                    if( rule.accepts(lexer, source, temp) )
                    {
                        found = true;
                        if(0!=temp)
                        {
                            XNode::Grow(tree,temp);
                            break;
                        }
                        // else try to capture a further robust rule
                    }
                    assert(0==temp);
                }

                return found;
            }

        }

    }

}
