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
    

    scanner("INT","[:digit:]+");
    scanner("ID","[:alpha:][:word:]*");
    scanner("FLT","[:digit:]+f?");;
    scanner("blank","[:blank:]",false);

    
    if(argc>1)
    {

        Jive::Source           source( Jive::Module::Open(argv[1]) );
    SCAN:
        scanner.cleanup();
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

