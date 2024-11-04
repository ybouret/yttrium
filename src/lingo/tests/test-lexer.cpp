#include "y/lingo/lexer.hpp"
#include "y/utest/run.hpp"

#include "y/lingo/lexical/add-on/single-line-comment.hpp"

using namespace Yttrium;
using namespace Lingo;


namespace {


    class MyLexer : public Lexer
    {
    public:

        explicit MyLexer() : Lexer("MyLexer"), com()
        {

            

            emit("INT",   "[:digit:]+");
            emit("HEX",   "0x[:xdigit:]+");

            Analyzer &comment = decl( new Analyzer(*this,"comment") );

            call(comment.name,
                 '$',
                 *this,
                 &MyLexer::commentEnter,
                 Lexical::Unit::Bulk);

            comment.on("dot", "[:dot:]", *this, & MyLexer::commentStore);
            comment.back("[:endl:]", *this, &MyLexer::commentLeave, Lexical::Unit::Endl);


            const AddOn &shellComment = plug<Lexical::ShellComment>("ShellComment");
            std::cerr << "defining " << shellComment.name << std::endl;

            const AddOn &cppComment = plug<Lexical::SingleLineComment>("C++Comment","//");
            std::cerr << "defining " << cppComment.name << std::endl;


            endl("[:endl:]", Lexical::Unit::Drop);
            drop("[:blank:]");
        }

        virtual ~MyLexer() noexcept
        {

        }

        void commentEnter(const Token &token)
        {
            std::cerr << "<comment>" << std::endl;
            com.release();
            com += token;
        }

        Lexical::Outcome commentStore(const Token &token)
        {
            com += token;
            return Lexical::Outcome(Lexeme::Drop,Lexeme::Bulk);
        }



        void commentLeave(const Token &token)
        {
            com += token;
            std::cerr << "\t[" << com.toPrintable() << "]" << std::endl;
            std::cerr << "<comment/>" << std::endl;
        }

        Token com;

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
