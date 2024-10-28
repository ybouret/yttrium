
#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/lexical/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"
#include "y/system/exception.hpp"

#include "y/lingo/lexical/action/all.hpp"


using namespace Yttrium;
using namespace Lingo;

Y_UTEST(scanner)
{
    Y_SIZEOF(Lexical::Unit);
    Y_SIZEOF(Lexical::Rule);
    Y_SIZEOF(Lexical::Scanner);
    Y_SIZEOF(Lexical::RList);
    Y_SIZEOF(Lexical::Emit);
    Y_SIZEOF(Lexical::Drop);
    Y_SIZEOF(Lexical::Back);
    Y_SIZEOF(Lexical::Call);



    Dictionary            dict;
    Lexical::Scanner      scan("MyScanner",dict);
    const Lexical::Action emit   = new Lexical::Emit(false);
    const Lexical::Action emitNL = new Lexical::Emit(true);
    const Lexical::Action drop   = new Lexical::Drop(false);
    const Lexical::Action dropNL = new Lexical::Drop(true);

    scan(Lexical::Rule::Create("FLT", scan.compile("[:digit:]+f"), emit));
    //scan(Lexical::Rule::Create("INT", scan.compile("[:digit:]+") ) );
    //scan(Lexical::Rule::Create("HEX", scan.compile("0x[:xdigit:]+") ) );

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
