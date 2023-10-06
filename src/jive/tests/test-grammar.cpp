
#include "y/jive/grammar.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyGrammar : public  Grammar
    {
    public:
        explicit MyGrammar() :  Grammar("MyGrammar")
        {

            Agg        &EXPR = add( new Agg("EXPR") );
            const Rule &INT  = add( new Syntax::Terminal("INT") );
            const Rule &SEP  = add( new Syntax::Terminal("SEP") );

            const Rule &ID   = add( new Syntax::Terminal("ID") );

            EXPR << Pick(INT,ID) << SEP;

            validate();
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
        explicit MyLexer() : Lexer("MyLexer") 
        {
            emit("INT","[0-9]+");
            emit("SEP",';');
            emit("ID","[:word:]+");
            endl("[:endl:]");
            drop("[:blank:]");
        }

        virtual ~MyLexer() noexcept {}



    private:
        Y_DISABLE_ASSIGN(MyLexer);
    };
}

Y_UTEST(grammar)
{


    MyGrammar G;
    MyLexer   L;

    Vizible::GraphViz("grammar.dot", G, true);




}
Y_UDONE()


