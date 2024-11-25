
#include "y/chemical/weasel/formula/linker.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: Linker:: ~Linker() noexcept
        {
        }

        Formula:: Linker:: Linker(const Weasel::Parser &parser) :
        Lingo::Syntax::Translator(parser.FORMULA.name),
        z(0)
        {
            Y_Lingo_OnTerminal(Linker,ELEMENT);
            Y_Lingo_OnTerminal(Linker,INTEGER);
            onTerminal("+", *this, & Linker:: onPLUS);
            onTerminal("-", *this, & Linker:: onMINUS);

            Y_Lingo_OnInternal(Linker,FORMULA);
            Y_Lingo_OnInternal(Linker,STOCHIO);
            Y_Lingo_OnInternal(Linker,Z);

        }

        void Formula::Linker:: clear() noexcept
        {
            elements.free();
            integers.free();
            zcharges.free();
            z = 0;
        }


        void Formula:: Linker:: init()
        {
            clear();
        }

        void Formula:: Linker:: quit()
        {
            std::cerr << "quit@" << elements << ", z=" << z << std::endl;
            assert(1==elements.size());

        }

        void Formula:: Linker:: onELEMENT(const Lingo::Lexeme &lexeme)
        {
            elements << lexeme.toString();
        }

        void Formula:: Linker:: onINTEGER(const Lingo::Lexeme &lexeme)
        {
            static const char fn[] = "coefficient";
            const String    s = lexeme.toString();
            const unsigned  n = ASCII::Convert::To<unsigned>(s,fn);
            if(n<=0) throw Specific::Exception( name->c_str(), "forbidden zero %s",fn);

            integers << n;
        }

        void Formula:: Linker:: onPLUS(const Lingo::Lexeme &)
        {
            zcharges << PositiveCharge;
        }

        void Formula:: Linker:: onMINUS(const Lingo::Lexeme &)
        {
            zcharges << NegativeCharge;
        }

        void Formula:: Linker:: onFORMULA(const size_t n)
        {
            assert(n>0);
            assert(n<=elements.size());
            const bool needParens = (deep>0);
            String res;
            if(needParens)
                res += '(';
            const size_t m = elements.size();
            for(size_t i=1+(m-n);i<=m;++i)
            {
                res += elements[i];
            }
            if(needParens)
                res += ')';

            for(size_t i=n;i>0;--i) elements.popTail();

            elements << res;
        }

        void Formula:: Linker:: onSTOCHIO(const size_t)
        {
            assert(elements.size()>0);
            assert(integers.size()>0);
            String          s = elements.pullTail();
            const unsigned  w = integers.pullTail();
            s += Formatted::Get("%u",w);
            elements << s;
        }

        void Formula:: Linker:: onZ(const size_t n)
        {
            assert(zcharges.size()>0);

            String        target = '^';
            const Charge  charge = zcharges.pullTail();
            unsigned      number = 1;
            
            switch(n)
            {
                case 1: // no coef
                    break;

                case 2: // with coef
                    assert(integers.size()>0);
                    target += Formatted::Get("%u", number=integers.pullTail() );
                    break;

                default:
                    throw Specific::Exception(name->c_str(), "onZ not implemented");
            }

            switch(charge)
            {
                case PositiveCharge:
                    target += '+';
                    z = int(number);
                    break;

                case NegativeCharge:
                    target += '-';
                    z = -int(number);
                    break;
            }

            elements << target;
        }

    }

}
