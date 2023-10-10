#include "y/jive/parser.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

//#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"

using namespace Yttrium;

/*
 add  : mult ('+'^ mult)* ;        // left association
 mult : pow ('*'^ pow)* ;          // left association
 pow  : atom ('^'^ pow)? ;         // right association
 atom : ID | INT | '('^ add ')'! ; // recursion
 */
namespace
{
    class Eval : public Jive::Parser
    {
    public:
        explicit Eval() : Jive::Parser("Eval")
        {
            Agg &        ADDOP  = act("ADDOP");
            Agg &        MULOP  = act("MULOP");
            Agg &        POWER  = act("POWER");
            const Rule & ID     = term("ID","[:alpha:][:word:]*");
            const Rule & INT    = term("INT","[:digit:]+");
            const Rule & LPAREN = mark('(',"\\(");
            const Rule & RPAREN = mark(')',"\\)");
            const Rule & PLUS   = term('+',"\\+");
            const Rule & MINUS  = term('-');
            const Rule & MUL    = term('*',"\\*");
            const Rule & DIV    = term('/');
            const Rule & POW    = mark('^');

            Alt &ATOM  = alt("ATOM") << ID << INT << cat(LPAREN,ADDOP,RPAREN);

            ADDOP += MULOP;
            ADDOP += zom(cat( pick(PLUS,MINUS), MULOP ));

            MULOP += POWER;
            ADDOP += zom(cat( pick(MUL,DIV), POWER ));

            POWER += ATOM;
            POWER += opt( cat(POW,POWER) );

            lexer.drop("[:blank:]+");
            lexer.endl("[:endl:]");

            renderGraphViz();
            validate();
        }

        virtual ~Eval() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };

}


Y_UTEST(eval)
{
    Eval eval;
    Jive::Syntax::XTree ast = eval( Jive::Module::Open( argv[1]) );
    if(ast.isValid())
    {
        Vizible::GraphViz( *eval.name + "-ast.dot", *ast);
    }
}
Y_UDONE()

