#include "y/lingo/lexer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Lingo;

Y_UTEST(lexer)
{
    Lexer lexer("MyLexer");
    std::cerr << "using " << lexer.name << std::endl;
}
Y_UDONE()
