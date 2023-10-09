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

            VALUE << term("NUMBER","[:digit:]+");

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


            renderGraphViz();
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON);
    };
}

Y_UTEST(parser)
{

    JSON json;
    std::cerr << json.name << std::endl;

}
Y_UDONE()

