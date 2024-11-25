
#include "y/chemical/formula.hpp"
#include "y/lingo/syntax/xlist.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: ~Formula() noexcept
        {

        }

        Formula:: Formula(XNode * const node) noexcept :
        code(node)
        {
            assert(!code.isEmpty());
            walk(node,0);
        }

        Formula::ConstInterface & Formula:: surrogate() const noexcept
        {
            assert( code.isValid() );
            return *code;
        }



        void Formula:: walk(const XNode * const xnode,
                            unsigned            level)
        {
            assert(0!=xnode);
            switch(xnode->type)
            {
                case XNode:: Terminal:
                    Core::Indent(std::cerr << std::setw(3) << level << "|",level) << xnode->lexeme() << std::endl;
                    break;

                case XNode:: Internal:
                    Core::Indent(std::cerr << std::setw(3) << level << "|",level) << xnode->name() << std::endl;
                    ++level;
                    for(const XNode *node=xnode->branch().head;node;node=node->next)
                    {
                        walk(node,level);
                    }
                    break;
            }
        }


    }

}


