
#include "y/jive/grammar.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

#include "y/jive/lexical/plugin/single-line-comment.hpp"


using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyGrammar : public  Grammar
    {
    public:
        explicit MyGrammar() :  Grammar("MyGrammar")
        {

            Agg        &EXPR = agg("EXPR");
            const Rule &INT  = add( new Syntax::Terminal("INT") );
            const Rule &SEP  = add( new Syntax::Terminal("SEP",Term::IsDivider,true) );

            const Rule &ID   = add( new Syntax::Terminal("ID") );

            EXPR << pick(INT,ID) << opt(SEP);

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
            plug<Lexical::HashTagComment>("com");
        }

        virtual ~MyLexer() noexcept {}



    private:
        Y_DISABLE_ASSIGN(MyLexer);
    };
}

#include "y/jive/source.hpp"
Y_UTEST(grammar)
{


    MyGrammar G;
    MyLexer   L;

    Jive::Syntax::Rule::Verbose = true;

    G.renderGraphViz(true);

    if(argc>1)
    {
        Jive::Source source( Jive::Module::Open(argv[1]) );
        AutoPtr<Syntax::XNode> tree = G.parse(L,source);
        std::cerr << "tree isValid=" << tree.isValid() << std::endl;
        if(tree.isValid())
        {
            GraphViz::Vizible::DotToPng("grammar-tree.dot",*tree,true);
        }
    }




}
Y_UDONE()


