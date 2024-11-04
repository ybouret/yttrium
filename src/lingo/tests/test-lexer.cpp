#include "y/lingo/lexer.hpp"
#include "y/utest/run.hpp"

#include "y/lingo/lexical/add-on/single-line-comment.hpp"
#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"

using namespace Yttrium;
using namespace Lingo;


namespace {

    class Verbatim : public Lexical::AddOn
    {
    public:
        template <typename LABEL> inline
        explicit Verbatim(Lexer &       rootLexer,
                          const LABEL & addOnUUID) :
        Lexical::AddOn(rootLexer,addOnUUID,"<v>","</v>",Lexeme::Bulk,RejectEndOfSource),
        content()
        {
            on("[:dot:]",  *this, & Verbatim::store<Lexeme::Bulk> );
            on("[:endl:]", *this, & Verbatim::store<Lexeme::Endl> );
        }

        virtual ~Verbatim() {}


        Token content;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Verbatim);
        virtual void enter(const Token &) noexcept
        {
            content.release();
        }

        virtual void leave(const Token &) noexcept
        {
            lexer.put( produce(content) );
        }

        template <Lexeme::Spot SPOT>
        Lexical::Outcome store(const Token &token)
        {
            content += token;
            return Lexical::Outcome(Lexeme::Drop,SPOT);
        }

    };

    class MyLexer : public Lexer
    {
    public:

        explicit MyLexer() : Lexer("MyLexer"), com()
        {

            emit("INT",   "[:digit:]+");
            emit("HEX",   "0x[:xdigit:]+");

            // manual definition
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

            const AddOn &cComment   = plug<Lexical::C_Comment>("C_Comment");
            std::cerr << "defining " << cComment.name << std::endl;

            (void)plug<Verbatim>("verbatim");
            (void)plug<Lexical::MultiLinesComment>("XML_Comment","<!--","-->");


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
