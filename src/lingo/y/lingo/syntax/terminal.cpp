
#include "y/lingo/syntax/terminal.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Terminal:: ~Terminal() noexcept {}


            bool Rule:: isTerminal() const noexcept
            {
                return Terminal::UUID == uuid;
            }

            bool Rule:: isInternal() const noexcept
            {
                return Terminal::UUID != uuid;
            }

            XNode::Type Rule:: typeOfNode() const noexcept
            {
                return Terminal::UUID == uuid ? XNode::Terminal : XNode::Internal;
            }

            void Terminal:: vizMark(OutputStream &fp) const
            {
                fp << ",shape=box";
                fp << ",style=\"filled";
                switch(kind)
                {
                    case Univocal: fp << ",rounded"; break;
                    case Standard: break;
                }

                switch(role)
                {
                    case Semantic: break;
                    case Dividing: fp << ",dashed"; break;
                }
                fp << "\"";
            }

            bool Terminal:: accepts(Y_Lingo_Syntax_Args) const
            {
                Lexeme * const lexeme = lexer.get(source);
                if(0==lexeme) return false; // EOF
                if( *(lexeme->name) != *name ) {
                    lexer.put(lexeme);
                    return false;
                }
                XNode::Grow(tree, XNode::CreateFrom(*this,lexeme) );
                assert(tree->isWellFormed());
                return true;
            }

            bool Terminal:: robust() const
            {
                return true;
            }

        }
    }
}

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            XNode:: XNode(const Syntax::Terminal & _rule, Lexeme * const _unit) noexcept:
            Object(),
            rule(_rule),
            type(Terminal),
            sire(0),
            next(0),
            prev(0)
            {
                assert(0!=_unit);
                assert(Terminal==rule.typeOfNode());
                zero();
                unit = _unit;
            }
        }

    }

}

