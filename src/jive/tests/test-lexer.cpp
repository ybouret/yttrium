
#include "y/jive/lexer.hpp"
#include "y/utest/run.hpp"
#include "y/jive/source.hpp"

using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
        }

        virtual ~MyLexer() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
}

Y_UTEST(lexer)
{
    Lexical::Scanner::Verbose = true;

    MyLexer lexer;
    

}
Y_UDONE()

