#include "y/lingo/parser.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

/*
 add  : mult ('+'^ mult)* ; // left association
 mult : pow ('*'^ pow)* ; // left association
 pow  : atom ('^'^ pow)? ; // right association
 atom : ID | INT | '('^ add ')'! ; // recursion
 */

namespace Yttrium
{
    class Eval : public Lingo::Parser
    {
    public:
        explicit Eval() : Lingo::Parser("Eval")
        {

            Agg & MulOp = act("MulOp");
            Agg & AddOp = act("AddOp");
            Agg & Pow   = act("Pow");
            Alt & Atom  = alt("Atom");

            const Rule & MINUS  = term('-');
            const Rule & PLUS   = term('+');
            const Rule & DIV    = term('/');
            const Rule & MUL    = term('*');
            const Rule & LPAREN = mark('(');
            const Rule & RPAREN = mark(')');

            AddOp << MulOp << zom( cat( pick(PLUS,MINUS), MulOp) );
            MulOp << Pow   << zom( cat( pick(MUL,DIV),    Pow)   );
            Pow   << Atom  << opt( cat(mark('^'),Pow ) );

            Atom << term("INT","[:digit:]+") << term("ID","[:alpha:][:word:]*") << cat( LPAREN, AddOp, RPAREN );

            top( agg(name) << AddOp);

            render();

            validate();
            
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]",Drop);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };
}

Y_UTEST(eval)
{
    Eval eval;
    if(argc>1)
    {
        AutoPtr<Lingo::Syntax::XNode> xnode = eval( Lingo::Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("eval-ast.dot", *xnode);
        xnode->toBinary("eval.dat");
    }

}
Y_UDONE()

