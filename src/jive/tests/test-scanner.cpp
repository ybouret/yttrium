#include "y/jive/lexical/scanner.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(scanner)
{
    Jive::Lexical::Scanner scanner("demo");

    Y_SIZEOF(Jive::Dictionary);

    scanner("INT","[:digit:]+", scanner, & Jive::Lexical::Scanner::produce);

}
Y_UDONE()

