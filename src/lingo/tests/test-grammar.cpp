
#include "y/lingo/syntax/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace
{

    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
            emit("INT", "[:digit:]+");
            drop("[:blank:]");
            endl("[:endl:]", Lexeme::Drop );
        }

        virtual ~MyLexer() noexcept
        {
        }
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };

    class MyGrammar : public Syntax::Grammar
    {
    public:
        explicit MyGrammar() : Syntax::Grammar("MyGrammar")
        {
            term_("INT", Syntax::Terminal::Standard,  Syntax::Terminal::Semantic);
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
    MyGrammar G;
    MyLexer   L;

    G.render();
    if(argc>1)
    {
        Source                 source( Lingo::Module::OpenFile(argv[1]));
        AutoPtr<Syntax::XNode> xnode = G.accept(L,source);

        if(xnode.isValid())
        {
            GraphViz::Vizible::DotToPng("xnode.dot", *xnode);
            
        }
        else
        {
            std::cerr << "no tree" << std::endl;
        }
    }
}
Y_UDONE()

