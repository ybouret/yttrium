#include "y/jive/parser.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/syntax/analyzer.hpp"

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
            Alt        & VALUE   = alt("VALUE");
            const Rule & COMMA   = mark(',');
            const Rule & LBRACK  = mark("[", "\\[");
            const Rule & RBRACK  = mark("]","\\]");
            const Rule & STRING  = plug<Jive::Lexical::JString>("STRING");
            VALUE 
            << term("NUMBER","[:digit:]+")
            << STRING
            << term("true")
            << term("false")
            << term("null")
            ;
            
            Alt & ARRAY = alt("ARRAY");
            {
                ARRAY << ( agg("EmptyArray") << LBRACK << RBRACK);
                {
                    Agg &HeavyArray = agg("HeavyArray");
                    HeavyArray << LBRACK;
                    HeavyArray << VALUE;
                    HeavyArray << zom( cat(COMMA,VALUE) );
                    HeavyArray << RBRACK;
                    ARRAY << HeavyArray;
                }
                VALUE << ARRAY;
            }
            G << ARRAY;

            const Rule & LBRACE = mark('{',"\\{");
            const Rule & RBRACE = mark('}',"\\}");


            Alt & OBJECT = alt("OBJECT");
            {
                OBJECT |= ( agg("EmptyObject") << LBRACE << RBRACE);
                {
                    Agg &HeavyObject = agg("HeavyObject");

                    HeavyObject += LBRACE;
                    const Rule &Pair = agg("Pair") << STRING << mark(':') << VALUE;
                    HeavyObject += Pair;
                    HeavyObject += zom( cat(COMMA,Pair) );
                    HeavyObject += RBRACE;

                    OBJECT |= HeavyObject;
                }

                VALUE << OBJECT;
            }

            G << OBJECT;

            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");

            renderGraphViz();
            validate();
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
            Jive::Syntax::Analyzer  analyzer;
            analyzer.run(*ast);
        }
    }

}
Y_UDONE()

