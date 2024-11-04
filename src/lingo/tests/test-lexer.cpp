#include "y/lingo/lexer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Lingo;


namespace {


    class MyLexer : public Lexer
    {
    public:

        explicit MyLexer() : Lexer("MyLexer")
        {

            
            //Analyzer &comment = decl( new Analyzer(*this,"comment") );

            emit("INT",   "[:digit:]+");
            emit("HEX",   "0x[:xdigit:]+");
            endl("ENDL",  "[:endl:]", Lexical::Unit::Drop);
            drop("BLANK", "[:blank:]");
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
    MyLexer lexer;
    std::cerr << "using " << lexer.name << std::endl;

    if(argc>1)
    {
        Source source( Module::OpenFile(argv[1]) );

        AutoPtr<Lexeme> lexeme = 0;

        while( (lexeme = lexer.get(source) ).isValid() )
        {
            std::cerr << lexeme << std::endl;
        }


    }


}
Y_UDONE()
