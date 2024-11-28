
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
                Y_Lingo_Syntax_Rule_Emit(depth,"[Terminal '" << name << "']");
                Lexeme * const lexeme = lexer.get(source,0);
                if(0==lexeme)
                {
                    Y_Lingo_Syntax_Rule_Emit(depth,"|_<EOF>");
                    return false; // EOF
                }

                if( *(lexeme->name) != *name ) {
                    lexer.put(lexeme);
                    Y_Lingo_Syntax_Rule_Emit(depth,"|_<reject " << *lexeme << ">");
                    return false;
                }

                Y_Lingo_Syntax_Rule_Emit(depth,"|_<accept " << *lexeme << ">");
                XNode::Grow(tree, XNode::CreateFrom(*this,lexeme) );
                assert(tree->isWellFormed());
                return true;
            }

            bool Terminal:: robust() const
            {
                return true;
            }

            Lexeme * Terminal:: synthetic(const String &input) const
            {
                Context         context(name,AsCaption);
                AutoPtr<Lexeme> lexeme = new Lexeme(*this,context);

                {
                    const char * p = input.c_str();
                    for(size_t i=input.size();i>0;--i)
                    {
                        lexeme->pushTail( new Char(context,*(p++)) );
                        context.newChar();
                    }
                }
                return lexeme.yield();
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
            Counted(),
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

            void XNode:: appendTo(Exception &excp) const noexcept
            {
                assert(Terminal==type);
                lexeme().appendTo(excp,rule.as<Syntax::Terminal>()->kind == Syntax::Terminal::Univocal);
            }

            XNode * XNode:: CreateFrom(const Syntax::Terminal &term, const String &data)
            {
                return CreateFrom(term,term.synthetic(data) );
            }

        }

    }

}

