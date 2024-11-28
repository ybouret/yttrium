
#include "y/chemical/weasel/formula/html.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Formula:: ToHTML:: ToHTML(const Weasel::Parser &parser) :
        Lingo::Syntax::Translator( *(parser.FORMULA.name) + "::ToHTML" ),
        elements(),
        integers(),
        zcharges()
        {
            Y_Lingo_OnTerminal(ToHTML,ELEMENT);
            Y_Lingo_OnTerminal(ToHTML,INTEGER);
            onTerminal('+', *this, & ToHTML::onZCHARGE);
            onTerminal('-', *this, & ToHTML::onZCHARGE);

            Y_Lingo_OnInternal(ToHTML,FORMULA);
            Y_Lingo_OnInternal(ToHTML,STOCHIO);
            Y_Lingo_OnInternal(ToHTML,Z);


        }

        Formula:: ToHTML:: ~ToHTML() noexcept
        {
        }

        void Formula:: ToHTML:: clear() noexcept
        {
            elements.free();
            integers.free();
            zcharges.free();
        }

        void Formula:: ToHTML:: init()
        {
            clear();
        }

        void Formula:: ToHTML:: quit()
        {
            if(1!=elements.size())
                throw Specific::Exception(name->c_str(), "corrupted formula");
        }


        void Formula:: ToHTML:: onELEMENT(const Lexeme &lexeme)
        {
            elements << lexeme.toString();
        }

        void Formula:: ToHTML:: onINTEGER(const Lexeme &lexeme)
        {
            integers << lexeme.toString();
        }

        void Formula:: ToHTML:: onZCHARGE(const Lexeme &lexeme)
        {
            zcharges << *lexeme.name;
        }

        void Formula:: ToHTML:: onFORMULA(const size_t n)
        {
            assert(n>0);
            assert( elements.size() >= n );
            String     elem;
            {
                const bool needParens = deep > 0;
                if(needParens) elem << '(';
                {
                    const size_t m = elements.size();
                    for(size_t i=1+(m-n);i<=m;++i)
                    {
                        elem << elements[i];
                    }
                    for(size_t i=n;i>0;--i) elements.popTail();
                }
                if(needParens) elem << ')';
            }
            elements << elem;
        }

        void Formula:: ToHTML:: onSTOCHIO(const size_t
#ifndef NDEBUG
                                          n
#endif
                                          )
        {
            assert(2==n);
            assert(elements.size()>0);
            assert(integers.size()>0);
            const String elem = elements.pullTail() + "<SUB>" + integers.pullTail() + "</SUB>";
            elements << elem;
        }

        void Formula:: ToHTML:: onZ(const size_t n)
        {
            assert(zcharges.size()>0);

            String z = "<SUP>";
            switch(n)
            {
                case 2:
                    assert( integers.size() > 0 );
                    z += integers.pullTail();
                    // FALLTHRU
                case 1:
                    z += zcharges.pullTail();
                    break;

                default:
                    throw Specific::Exception(name->c_str(), "invalid Z/%u", unsigned(n));
            }
            z += "</SUP>";
            elements << z;
        }

    }

}

