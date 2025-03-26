#include "y/chemical/weasel/formula/to-html.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Weasel:: FormulaToHTML:: ~FormulaToHTML() noexcept
        {
        }

        const char * const Weasel:: FormulaToHTML:: CallSign = "FormulaToHTML";



#define OnTerminal(NAME) Y_Lingo_OnTerminal(FormulaToHTML,NAME)
#define OnInternal(NAME) Y_Lingo_OnInternal(FormulaToHTML,NAME)

        Weasel:: FormulaToHTML:: FormulaToHTML() :
        FormulaCompiler(CallSign),
        str(),
        uid(0)
        {
            OnTerminal(Name);
            OnTerminal(Coef);
            onTerminal('+', *this, & FormulaToHTML:: onPos);
            onTerminal('-', *this, & FormulaToHTML:: onNeg);

            OnInternal(Z);
            OnInternal(Mult);
            OnInternal(Body);
            OnInternal(Formula);
        }

        void Weasel:: FormulaToHTML:: init()
        {
            str.release();
            uid.erase();
        }

        void Weasel:: FormulaToHTML:: quit()
        {
            assert(0==str.size());
            assert(uid.isValid());
            str.release();
        }

        void Weasel:: FormulaToHTML:: show() const
        {
            if(verbose)
            {
                if(str.size()>0)
                {
                    std::cerr << "str=" << str << std::endl;
                }
                else
                {
                    std::cerr << "html='" << uid << "'" << std::endl;
                }
            }
        }


        const String * Weasel:: FormulaToHTML:: get(const XNode &node)
        {
            assert( Formula::CallSign == node.name() );
            Translator &self = *this;
            //self.policy = Lingo::Syntax::Permissive;
            //self.verbose = true;
            self(node);
            assert(uid.isValid());
            return uid.yield();
        }

        void Weasel:: FormulaToHTML:: onName(const Lexeme &lx)
        {
            str << lx.toString();
        }


        void Weasel:: FormulaToHTML:: onCoef(const Lexeme &lx)
        {
            str << lx.toString();
        }

        void Weasel:: FormulaToHTML:: onPos(const Lexeme &)
        {
            str << '+';
        }

        void Weasel:: FormulaToHTML:: onNeg(const Lexeme &)
        {
            str << '-';
        }

        void Weasel:: FormulaToHTML:: onZ(const size_t n)
        {
            assert(n==1||n==2);
            assert(str.size()>=n);
            String s = "<SUP>";
            {
                const size_t m=str.size()+1;
                for(size_t i=n;i>0;--i)
                {
                    s += str[m-i];
                }
            }
            s += "</SUP>";
            str.trim(n);
            str <<  s;
        }

        void Weasel:: FormulaToHTML:: onMult(const size_t)
        {
            assert(str.size()>=2);
            const size_t m = str.size();
            String s = str[m-1];
            s += "<SUB>";
            s += str[m];
            s += "</SUB>";
            str.trim(2);
            str << s;
        }

        void Weasel:: FormulaToHTML:: onBody(const size_t n)
        {
            assert(str.size()>=n);
            const bool   parens = deep>1;
            const size_t m      = str.size()+1;
            String       s;
            if(parens) s += '(';
            for(size_t i=n;i>0;--i)
            {
                s += str[m-i];
            }
            if(parens) s += ')';
            str.trim(n);
            str << s;
        }

        void Weasel:: FormulaToHTML:: onFormula(const size_t)
        {
            //assert(n==str.size());
            uid = new String();
            while(str.size()>0)
            {
                *uid += str.head(); str.popHead();
            }
        }
    }

}


