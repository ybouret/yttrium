
#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(scanner)
{
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Lexical::Rule);
    Y_SIZEOF(Lexical::Scanner);
    Y_SIZEOF(Lexical::Scanner::RList);

    Dictionary       dict;
    Lexical::Scanner scan("MyScanner",dict);

}
Y_UDONE()
