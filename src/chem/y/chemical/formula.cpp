
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
        text( new String() ),
        code(node)
        {
            assert(!code.isEmpty());
            walk(node,0);
            std::cerr << "text=" << text << std::endl;
        }

        Formula::ConstInterface & Formula:: surrogate() const noexcept
        {
            assert( code.isValid() );
            return *code;
        }

        void Formula:: add(const Lingo::Lexeme &lexeme)
        {
            const String rhs = lexeme.toString();
            Coerce( *text ) << rhs;
        }


        void Formula:: walk(const XNode * const xnode,
                            unsigned            level)
        {
            assert(0!=xnode);
            const String & uuid = xnode->name();
            switch(xnode->type)
            {
                case XNode:: Terminal:
                    Core::Indent(std::cerr << std::setw(3) << level << "| term ",level) << xnode->lexeme() << std::endl;
                    add(xnode->lexeme());
                    break;

                case XNode:: Internal:
                    Core::Indent(std::cerr << std::setw(3) << level << "| intr ",level) << xnode->name() << std::endl;
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


