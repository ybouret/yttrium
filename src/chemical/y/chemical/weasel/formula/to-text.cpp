
#include "y/chemical/weasel/formula/to-text.hpp"
#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
#define OnTerminal(NAME) Y_Lingo_OnTerminal(FormulaToText,NAME)
#define OnInternal(NAME) Y_Lingo_OnInternal(FormulaToText,NAME)

        Weasel:: FormulaToText:: FormulaToText() :
        Lingo::Syntax::Translator("FormulaToText"),
        str(),
        cof(),
        sgn(),
        uid(0),
        zzz(0)
        {
            OnTerminal(Name);
            OnTerminal(Coef);

            OnInternal(Mult);
            OnInternal(Body);
            OnInternal(Z);

            onTerminal('+', *this, & FormulaToText:: onPos);
            onTerminal('-', *this, & FormulaToText:: onNeg);


        }

        Weasel:: FormulaToText:: ~FormulaToText() noexcept
        {

        }


        void Weasel:: FormulaToText:: reset() noexcept
        {
            str.release();
            cof.release();
            sgn.release();
            uid.erase();
            zzz=0;
        }

        void Weasel:: FormulaToText:: init()
        {
            reset();
        }

        void Weasel:: FormulaToText:: quit()
        {
            std::cerr << "Now Quitting [" << str << "]" << std::endl;
        }


        void Weasel:: FormulaToText:: onName(const Lexeme &lexeme)
        {
            str << lexeme.toString();
            indent() << "str=" << str << std::endl;
        }


        void Weasel:: FormulaToText:: onCoef(const Lexeme &lexeme)
        {
            const apn value = lexeme.toNatural();
            cof << value.toDec();
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
            String       body;
            {
                const bool   sub = deep>1;
                const size_t off = str.size()+1;
                if(sub) body += '(';
                for(size_t i=n;i>0;--i)
                    body += str[off-i];
                if(sub) body += ')';
            }
            str.trim(n);
            str << body;
            indent() << "str=" << str << std::endl;
        }

        void Weasel:: FormulaToText:: onPos(const Lexeme &)
        {
            sgn << '+';
        }

        void Weasel:: FormulaToText:: onNeg(const Lexeme &)
        {
            sgn << '-';
        }

        void Weasel:: FormulaToText:: onZ(const size_t n)
        {
            assert(n==1||n==2);
            String &body = str.tail();
            body << '^';
            switch(n)
            {
                case 2: assert(cof.size()>0); body << cof.pullTail(); // FALLTHRU
                case 1: assert(sgn.size()>0); body << sgn.pullTail(); break;
            }
            indent() << "str=" << str << std::endl;
        }

    }

}
