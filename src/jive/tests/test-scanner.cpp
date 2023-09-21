#include "y/jive/lexical/scanner.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(scanner)
{
    Jive::Lexical::Scanner scanner("demo");

    Y_SIZEOF(Jive::Dictionary);
    Y_SIZEOF(Jive::Lexical::Callback);
    

    scanner("INT","[:digit:]+", scanner, & Jive::Lexical::Scanner::produce);
    
    scanner.cleanup();
    

}
Y_UDONE()

