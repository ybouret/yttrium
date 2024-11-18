
#include "y/lingo/syntax/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"

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
            emit(';');

            (void)lexer.plug<Lexical::ShellComment>("ShellComment");
            (void)lexer.plug<Lexical::JString>("STRING");

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
        explicit MyGrammar() : Syntax::Grammar("MyGrammar",0)
        {
            Agg & STATEMENT = agg("STATEMENT");
            const Rule & INT = term__("INT", Syntax::Terminal::Standard,  Syntax::Terminal::Semantic);
            const Rule & SEP = term__(";",   Syntax::Terminal::Univocal,  Syntax::Terminal::Dividing);
            STATEMENT << INT << SEP;

            Agg & TEXT       = act("TEXT");
            TEXT << term__("STRING",Syntax::Terminal::Standard,  Syntax::Terminal::Semantic);
            TEXT << SEP;

            top( zom(pick(STATEMENT,TEXT) ) );
            validate();
        }

        virtual ~MyGrammar() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyGrammar);
    };
}

#include "y/vfs/local/fs.hpp"

Y_UTEST(grammar)
{
    Syntax::Rule::Trace = true;
    LocalFS::Instance().tryRemoveFile("xnode-raw.png");
    LocalFS::Instance().tryRemoveFile("xnode-raw.png");
    MyGrammar G;
    MyLexer   L;


    G.render();
    if(argc>1)
    {
        Source                 source( Lingo::Module::OpenFile(argv[1]));
        AutoPtr<Syntax::XNode> xnode = G.accept(L,source);

        if(xnode.isValid())
        {
            GraphViz::Vizible::DotToPng("xnode-raw.dot", *xnode);
            xnode = Syntax::XNode::AST( xnode.yield() );
            GraphViz::Vizible::DotToPng("xnode-ast.dot", *xnode);
        }
        else
        {
            std::cerr << "no tree" << std::endl;
        }
    }
}
Y_UDONE()

