
#include "y/jive/lexer.hpp"
#include "y/utest/run.hpp"
#include "y/jive/source.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"

using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
            Scanner &self = *this;
            self("INT","[:digit:]+");
            self("ID","[:alpha:][:word:]*");
            self("FLT","[:digit:]+f?");;
            self("blank","[:blank:]+",false);
            self.endl("endl", "[:endl:]");

#if 1
            Scanner &com = make<Scanner>("com");
            self.call("com", "#", *this, self, &Scanner::discard);
            com("dot",".+",false);
            com.back("[:endl:]", *this, com, &Scanner::newLineAndDrop);
#endif

            plug<Lexical::CxxComment>("CxxComment");




        }

        virtual ~MyLexer() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
}

Y_UTEST(lexer)
{
    Lexical::Scanner::Verbose = true;

    MyLexer lexer;
    Lexemes lexemes;

    if(argc>1)
    {
        Source  src( Jive::Module::Open(argv[1]) );
        Lexeme *lxm = 0;
        while( 0 != (lxm=lexer.get(src) ) )
        {
            lexemes.pushTail(lxm);
        }

        for(const Lexical::Unit *unit=lexemes.head;unit;unit=unit->next)
        {
            std::cerr << *unit << std::endl;
        }
    }


}
Y_UDONE()

