
#include "y/chemical/weasel/formula/to-text.hpp"

namespace Yttrium
{
    namespace Chemical
    {
#define OnTerminal(NAME) Y_Lingo_OnTerminal(FormulaToText,NAME)
#define OnInternal(NAME) Y_Lingo_OnInternal(FormulaToText,NAME)

        Weasel:: FormulaToText:: FormulaToText() :
        Lingo::Syntax::Translator("FormulaToText"),
        str()
        {
            OnTerminal(Name);
            OnTerminal(Coef);
            OnInternal(Mult);
            OnInternal(Body);
        }

        Weasel:: FormulaToText:: ~FormulaToText() noexcept
        {

        }

        void Weasel:: FormulaToText:: init()
        {
            str.release();
            cof.release();
        }

        void Weasel:: FormulaToText:: quit()
        {
            std::cerr << "Now Quitting..." << std::endl;
        }


        void Weasel:: FormulaToText:: onName(const Lexeme &lexeme)
        {
            str << lexeme.toString();
            indent() << "str=" << str << std::endl;
        }


        void Weasel:: FormulaToText:: onCoef(const Lexeme &lexeme)
        {
            cof << lexeme.toString();
            indent() << "cof=" << cof << std::endl;
        }

        void Weasel:: FormulaToText:: onMult(const size_t)
        {
            assert( str.size() > 0 );
            assert( cof.size() > 0 );
            String mult = str.pullTail() + cof.pullTail();
            str << mult;
            indent() << "str=" << str << std::endl;
        }


        void Weasel:: FormulaToText:: onBody(const size_t n)
        {
            assert( str.size() >= n);
            const bool   sub = deep>1;
            const size_t off = str.size()+1;
            String       body;
            if(sub) body += '(';
            for(size_t i=n;i>0;--i)
            {
                body += str[off-i];
            }
            if(sub) body += ')';
            str.trim(n);
            str << body;
            indent() << "str=" << str << std::endl;

        }
    }

}
