#include "y/jive/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/jive/source.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(scanner)
{
    Lexical::Scanner::Verbose = true;
    Lexical::Scanner scanner("demo");



    Y_SIZEOF(Dictionary);
    Y_SIZEOF(Lexical::Callback);
    

    scanner("INT","[:digit:]+");
    scanner("ID","[:alpha:][:word:]*");
    scanner("FLT","[:digit:]+f?");;
    scanner("blank","[:blank:]+",false);
    scanner.endl("endl", "[:endl:]");

    scanner.cleanup();
    
    if(argc>1)
    {

        Lexical::Units lexemes;

        Source source( Jive::Module::Open(argv[1]) );
    SCAN:
        //scanner.cleanup();
        Lexical::Action *action = 0;
        switch( scanner.probe(source,action) )
        {
            case  Lexical::AtEndOfStream:
                Y_ASSERT(0==action);
                break;
                
            case  Lexical::ReturnSuccess: {
                Y_ASSERT(0!=action);
                const  Lexical::Message msg = action->doing( *(action->motif) );
                if( msg & Lexical::LX_ENDL ) source.newLine();

                action->motif->release();
            } goto SCAN;

            case  Lexical::ReturnFailure: {
                Y_ASSERT(0==action);
                std::cerr << "Failure!!" << std::endl;
            } break;

        }
    }


}
Y_UDONE()

