
#include "y/chemical/weasel/formula/to-text.hpp"
#include "y/apex/natural.hpp"
#include "y/text/ascii/convert.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String  *Weasel:: FormulaToText:: get(const XNode &node, int &z)
        {
            assert( Formula::CallSign == node.name() );

            Translator &self = *this;
            self(node);
            assert(uid.isValid());
            z = zzz;
            const String * const s = uid.yield();
            resetAll();
            return s;
        }


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

            OnInternal(Formula);

        }

        Weasel:: FormulaToText:: ~FormulaToText() noexcept
        {

        }

        void Weasel:: FormulaToText::resetVec() noexcept
        {
            str.release();
            cof.release();
            sgn.release();
        }

        void Weasel:: FormulaToText:: resetAll() noexcept
        {
            resetVec();
            uid.erase();
            zzz=0;
        }

        void Weasel:: FormulaToText:: init()
        {
            resetAll();
        }

        void Weasel:: FormulaToText:: quit()
        {
            assert(str.size()==1);
            uid = new String( str.tail() );
            resetVec();
        }


        void Weasel:: FormulaToText:: onName(const Lexeme &lexeme)
        {
            str << lexeme.toString();
        }


        void Weasel:: FormulaToText:: onCoef(const Lexeme &lexeme)
        {
            const apn value = lexeme.toNatural();
            cof << value.toDec();
        }

        void Weasel:: FormulaToText:: onMult(const size_t)
        {
            assert( str.size() > 0 );
            assert( cof.size() > 0 );
            String mult = str.pullTail() + cof.pullTail();
            str << mult;
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
            zzz = 1;
            switch(n)
            {
                case 2:
                    assert(cof.size()>0);
                    zzz = ASCII::Convert::To<int>(cof.tail(),Formula::Z);
                    body << cof.pullTail();
                    // FALLTHRU
                case 1:
                    assert(sgn.size()>0);
                    if('-' == sgn.tail()) zzz = -zzz;
                    body << sgn.pullTail(); break;
            }
        }

        void Weasel:: FormulaToText:: onFormula(const size_t) noexcept
        {

        }

    }

}
