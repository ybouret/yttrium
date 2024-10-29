
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
    Y_SIZEOF(Lexical::Outcome);;




    Dictionary            dict;
    Lexical::Scanner      scan("MyScanner",dict);


    scan.emit("FLT",   "[:digit:]+f");
    scan.emit("DIGIT", "[:digit:]");
    scan.emit("HEX",   "0x[:xdigit:]+");
    scan.endl("ENDL",  "[:endl:]", Lexical::Unit::Drop);
    scan.drop("BLANK", "[:blank:]");
    

    //scan(Lexical::Rule::Create("FLT", scan.compile("[:digit:]+f") ));
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
        Source                   source( Module::OpenFile(argv[1]) );
        Lexical::Scanner::Result result = Lexical::Scanner::Regular;

        bool            done   = false;

        while(!done)
        {
            AutoPtr<Lexeme> lexeme = scan.run(source,result);
            switch(result)
            {
                case Lexical::Scanner::Regular:
                    if(lexeme.isValid())
                    {
                        std::cerr << "Regular " << lexeme << std::endl;
                    }
                    else
                    {
                        std::cerr << "EOF" << std::endl;
                        done = true;
                    }
                    break;

                case Lexical::Scanner::Control:
                    Y_ASSERT(lexeme.isEmpty());
                    std::cerr << "Control" << std::endl;
                    break;

                case Lexical::Scanner::Failure:
                {
                    Y_ASSERT(lexeme.isValid());
                    const String        bad = lexeme->toPrintable();
                    Specific::Exception excp(scan.name->c_str(), "unexpected '%s'", bad.c_str());
                    lexeme->info.stamp(excp);
                    throw excp;
                }
            }
        }

    }


}
Y_UDONE()
