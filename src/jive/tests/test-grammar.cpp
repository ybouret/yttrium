
#include "y/jive/syntax/grammar.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyGrammar : public Syntax::Grammar
    {
    public:
        explicit MyGrammar() : Syntax::Grammar("MyGrammar")
        {

            Agg        &EXPR = add( new Agg("EXPR") );
            const Rule &INT  = add( new Syntax::Terminal("INT") );
            const Rule &SEP  = add( new Syntax::Terminal("SEP") );

            EXPR << INT << SEP;

        }


        virtual ~MyGrammar() noexcept
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyGrammar);
    };

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer") {}
        virtual ~MyLexer() noexcept {}

    private:
        Y_DISABLE_ASSIGN(MyLexer);
    };
}

Y_UTEST(grammar)
{


    MyGrammar G;






}
Y_UDONE()


