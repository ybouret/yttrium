
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

            Agg & JSON = agg(name);

            Alt & VALUE = alt("VALUE");
            


            render();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]", Lexeme::Drop);
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
}
Y_UDONE()

