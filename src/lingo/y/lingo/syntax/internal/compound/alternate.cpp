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

            

            bool Alternate:: robust() const
            {
                checkNotEmpty("Alternate::robust()",name->c_str());
                return true;
                
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    if( (**node).flimsy() ) return false;
                }
                return false;
            }

            bool Alternate:: accepts(Y_Lingo_Syntax_Args) const
            {
                assert(manifest.size>0);
                Y_Lingo_Syntax_Rule_Emit(depth, "[Alternate '" << name << "']");
                
                bool         foundRule = false;
                const size_t ruleDepth = depth+1;

                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    const Rule &rule = **node;
                    XNode      *temp = 0;
                    if( rule.accepts(lexer, source, temp, ruleDepth) )
                    {
                        foundRule = true;
                        if(0!=temp)
                        {
                            XNode::Grow(tree,temp);
                            break;
                        }
                        // else try to capture a further robust rule
                    }
                    assert(0==temp);
                }

                return foundRule;
            }

        }

    }

}
