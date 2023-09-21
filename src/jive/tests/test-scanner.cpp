#include "y/jive/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/jive/source.hpp"

using namespace Yttrium;

Y_UTEST(scanner)
{
    Jive::Lexical::Scanner::Verbose = true;
    Jive::Lexical::Scanner scanner("demo");



    Y_SIZEOF(Jive::Dictionary);
    Y_SIZEOF(Jive::Lexical::Callback);
    

    scanner("INT","[:digit:]+",        scanner, & Jive::Lexical::Scanner::produce);
    scanner("ID","[:alpha:][:word:]*", scanner, & Jive::Lexical::Scanner::produce);
    scanner("FLT","[:digit:]+f?",       scanner, & Jive::Lexical::Scanner::produce);

    scanner.cleanup();

    if(argc>1)
    {

        Jive::Source           source( Jive::Module::Open(argv[1]) );
    SCAN:
        Jive::Lexical::Action *action = 0;
        switch( scanner.probe(source,action) )
        {
            case Jive::Lexical::AtEndOfStream:
                break;
                
            case Jive::Lexical::ReturnSuccess: {

            } goto SCAN;

            case Jive::Lexical::ReturnFailure: {
                std::cerr << "Failure!!" << std::endl;
            } break;

        }
    }


}
Y_UDONE()

