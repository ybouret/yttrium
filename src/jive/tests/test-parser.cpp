#include "y/jive/parser.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

#include "y/jive/lexical/plugin/jstring.hpp"

using namespace Yttrium;

namespace
{
    class JSON : public Jive::Parser
    {
    public:
        virtual ~JSON() noexcept {}

        explicit JSON() : Jive::Parser("JSON")
        {
            Alt        & G       = alt(name);
            Agg        & VALUE   = agg("VALUE");
            const Rule & COMMA   = mark(',');
            const Rule & LBRACK  = mark("[", "\\[");
            const Rule & RBRACK  = mark("]","\\]");

            VALUE << term("NUMBER","[:digit:]+") << plug<Jive::Lexical::JString>("STRING");

            Alt & ARRAY = alt("ARRAY");
            {
                ARRAY << ( agg("EmptyArray") << LBRACK << RBRACK);
                Agg &HeavyArray = agg("HeavyArray");
                HeavyArray << LBRACK;
                HeavyArray << VALUE;
                HeavyArray << zom( agg("ExtraValue") << COMMA << VALUE );
                HeavyArray << RBRACK;
                ARRAY << HeavyArray;
            }
            G << ARRAY;


            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");

            renderGraphViz();
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON);
    };
}

Y_UTEST(parser)
{
    Jive::Syntax::Rule::Verbose = true;

    JSON json;
    std::cerr << json.name << std::endl;
    if(argc>1)
    {
        Jive::Syntax::XTree ast = json( Jive::Module::Open( argv[1]) );
        if(ast.isValid())
        {
            Vizible::GraphViz( *json.name + "-ast.dot", *ast);
        }
    }

}
Y_UDONE()

