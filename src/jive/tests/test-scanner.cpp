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
    

    scanner.emit("INT","[:digit:]+");
    scanner.emit("ID","[:alpha:][:word:]*");
    scanner.emit("FLT","[:digit:]+f?");;
    scanner.drop("blank","[:blank:]+");
    scanner.endl("endl", "[:endl:]");

    scanner.cleanup();

    if(argc>1)
    {

        Lexical::Units lexemes;

        Source source( Jive::Module::Open(argv[1]) );
    SCAN:
        Y_ASSERT(scanner.isClean());
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
                if( msg & Lexical::LX_EMIT ) lexemes.pushTail( action->produce() );
                action->motif->release();
            } goto SCAN;

            case  Lexical::ReturnFailure: {
                Y_ASSERT(0==action);
                std::cerr << "Failure!!" << std::endl;
            } break;

        }

        for(const Lexical::Unit *unit=lexemes.head;unit;unit=unit->next)
        {
            std::cerr << *unit << std::endl;
        }
    }


}
Y_UDONE()

