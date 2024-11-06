
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


            void Terminal:: viz(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                fp << "shape=box,style=\"filled";
                switch(kind)
                {
                    case Univocal: fp << ",bold"; break;
                    case Standard: break;
                }

                switch(role)
                {
                    case Semantic: break;
                    case Dividing: fp << ",dashed"; break;
                }
                Endl(fp << '\"' << ']');
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
                return true;
            }

            bool Terminal:: robust() const noexcept
            {
                return true;
            }

        }
    }
}

