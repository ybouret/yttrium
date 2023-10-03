
#include "y/jive/lexer.hpp"
#include "y/utest/run.hpp"
#include "y/jive/source.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"
#include "y/jive/lexical/plugin/bstring.hpp"

using namespace Yttrium;
using namespace Jive;

namespace
{
    class MyLexer : public Lexer
    {
    public:
        explicit MyLexer() : Lexer("MyLexer")
        {
            emit("INT","[:digit:]+");
            emit("ID","[:alpha:][:word:]*");
            emit("FLT","[:digit:]+f?");;
            drop("blank","[:blank:]+");
            endl("endl", "[:endl:]");

            plug<Lexical::CxxComment>("CxxComment");
            plug<Lexical::SingleLineComment>("ShellComment",'#');
            plug<Lexical::MultiLinesComment>("C_Comment","/\\*","\\*/");

            plug<Lexical::JString>("JString");
            plug<Lexical::RString>("RString");
            plug<Lexical::BString>("BString");



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

