
#include "y/lingo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace
{
    class JParser : public Parser
    {
    public:
        explicit JParser() : Parser("JSON")
        {

            Alt        & JSON   = alt(name);
            Alt        & VALUE  = alt("VALUE");
            const Rule & STRING = plug<Lexical::JString>("String");
            VALUE << STRING << term("Number", "[:digit:]+");

            Alt & ARRAY = alt("Array");
            {
                Agg        & HeavyArray = agg("HeavyArray");
                HeavyArray << '[';
                HeavyArray << VALUE;
                HeavyArray << zom( cat( get(','), VALUE) );
                HeavyArray << ']';
                ARRAY << HeavyArray;
                ARRAY << (agg("EmptyArray") << '[' << ']');
            }
            VALUE << ARRAY;

            Alt & OBJECT = alt("Object");
            {
                Agg & HeavyObject = agg("HeavyObject");
                HeavyObject << '{';
                const Rule & PAIR = agg("Pair") << STRING << ':' << VALUE;
                HeavyObject << PAIR;
                HeavyObject << zom( cat( get(','), PAIR) );
                HeavyObject << '}';

                OBJECT << HeavyObject;
                OBJECT << (agg("EmptyObject") << '{' << '}');
            }
            VALUE << OBJECT;

            // finish top-level
            JSON << ARRAY;
            JSON << OBJECT;

            VALUE << "null" << "true" << "false";
            render();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]", Lexeme::Drop);

            validate();
        }

        virtual ~JParser() noexcept
        {

        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JParser);
    };
}


Y_UTEST(parser)
{

    JParser J;
    if(argc>1)
    {
        AutoPtr<Syntax::XNode> xnode = J( Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("xnode.dot", *xnode);
    }
}
Y_UDONE()

