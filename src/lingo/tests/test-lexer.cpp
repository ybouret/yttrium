#include "y/lingo/lexer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Lingo;


namespace {


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
    MyLexer lexer;
    std::cerr << "using " << lexer.name << std::endl;
}
Y_UDONE()
