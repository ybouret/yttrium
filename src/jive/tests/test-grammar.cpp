
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
}

Y_UTEST(grammar)
{
    Y_SIZEOF(Syntax::XNode);
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Syntax::XNode::List);


    const Syntax::Terminal term("INT");

    MyGrammar G;



    //AutoPtr<Syntax::XNode> xn = Syntax::XNode::Create();



}
Y_UDONE()


