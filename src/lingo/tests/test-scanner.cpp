
#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"
#include "y/system/exception.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(scanner)
{
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Lexical::Rule);
    Y_SIZEOF(Lexical::Scanner);
    Y_SIZEOF(Lexical::RList);

    Dictionary       dict;
    Lexical::Scanner scan("MyScanner",dict);

    scan(Lexical::Rule::Create("INT", scan.compile("[:digit:]+") ) );

    for(const Lexical::Rule *rule=scan->head;rule;rule=rule->next)
    {
        std::cerr << rule->name << " = '" << rule->motif->regularExpression() << "'" << std::endl;
        ///GraphViz::Vizible::DotToPng( *(rule->name) + ".dot", *(rule->motif) );
        //rule->motif->toBinary( *(rule->name) + ".dat");
    }

    if(argc>1)
    {
        Source   source( Module::OpenFile(argv[1]) );
        unsigned report=0;
        AutoPtr<Lexeme> lexeme = scan.run(source,report);
        std::cerr << lexeme << std::endl;
        if(1==report) {
            assert(lexeme.isValid());
            const String bad = lexeme->toPrintable();
            Specific::Exception excp(scan.name->c_str(), "unexpected '%s'", bad.c_str());
            lexeme->info.stamp(excp);
            throw excp;
        }
        else
        {
            
        }
    }


}
Y_UDONE()
