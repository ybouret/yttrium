
#include "y/lingo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

#include "y/lingo/syntax/translator.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace
{

    class ParserNL : public Parser
    {
    public:

        explicit ParserNL() : Parser("ParserNL")
        {

            Alt        &DATA = alt("DATA");
            const Rule &INT = term("INT","[:digit:]+");
            const Rule &NL  = endl("NL","[:endl:]");
            DATA << zom( pick(INT,NL) );

            lexer.drop("[:blank:]");

            render();
            validate();
        }

        virtual ~ParserNL() noexcept
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ParserNL);
    };

}


Y_UTEST(parser_nl)
{

    ParserNL P;
    if(argc>1)
    {
        AutoPtr<Syntax::XNode> xnode = P( Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("xnode.dot", *xnode);
        xnode->toBinary("xnode.dat");

        if(false)
        {
            Source src( Module::OpenFile("xnode.dat") );
            AutoPtr<Syntax::XNode> reloaded = P.reload(src);
        }
    }

}
Y_UDONE()
