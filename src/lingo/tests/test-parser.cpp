
#include "y/lingo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"

#include "y/lingo/syntax/translator.hpp"
#include "y/lingo/syntax/xwalk.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace
{
    class JParser : public Parser
    {
    public:
        explicit JParser() : Parser("JSON")
        {

            Alt        & JSON   = alt(name);
            Alt        & VALUE  = alt("VALUE");
            const Rule & STRING = plug<Lexical::JString>("String");
            VALUE << STRING << term("Number", "[:digit:]+");

            Alt & ARRAY = alt("Array");
            {
                Agg        & HeavyArray = agg("HeavyArray");
                HeavyArray << '[';
                HeavyArray << VALUE;
                HeavyArray << zom( cat( get(','), VALUE) );
                HeavyArray << ']';
                ARRAY << HeavyArray;
                ARRAY << (agg("EmptyArray") << '[' << ']');
            }
            VALUE << ARRAY;

            Alt & OBJECT = alt("Object");
            {
                Agg & HeavyObject = agg("HeavyObject");
                HeavyObject << '{';
                const Rule & PAIR = agg("Pair") << STRING << ':' << VALUE;
                HeavyObject << PAIR;
                HeavyObject << zom( cat( get(','), PAIR) );
                HeavyObject << '}';

                OBJECT << HeavyObject;
                OBJECT << (agg("EmptyObject") << '{' << '}');
            }
            VALUE << OBJECT;

            // finish top-level
            JSON << ARRAY;
            JSON << OBJECT;

            VALUE << "null" << "true" << "false";
            render();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]", Lexeme::Drop);

            validate();
        }

        virtual ~JParser() noexcept
        {

        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(JParser);
    };

    class JCompiler : public Syntax::Translator
    {
    public:

        explicit JCompiler() : Syntax::Translator("JSON")
        {
            Y_Lingo_OnTerminal(JCompiler,String);
            Y_Lingo_OnInternal(JCompiler,Pair);
        }

        virtual ~JCompiler() noexcept
        {

        }

        void onString(const Lexeme &lexeme)
        {
            indent()  << "\tFound String = '" << lexeme.toPrintable() << "'" << std::endl;
        }

        void onPair(const size_t n)
        {
            indent() << "\tFound Pair #" << n << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(JCompiler);
    };
}


namespace
{
    struct XCall
    {
        inline void display(Syntax::XNode &node, const size_t deep)
        {
            Core::Indent(std::cerr,deep) << " display [" << node.name() << "]" << std::endl;
        }
    };

    static inline void XDisp(Syntax::XNode &node, const size_t deep)
    {
        Core::Indent(std::cerr,deep) << " x_disp  [" << node.name() << "]" << std::endl;

    }
}

Y_UTEST(parser)
{

    JParser J;
    J.printRules();

    Syntax::XWalk xwalk;
    XCall         call;
    xwalk.on("Pair", call, & XCall::display);
    xwalk.on("String", XDisp);

    if(argc>1)
    {
        AutoPtr<Syntax::XNode> xnode = J( Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("xnode.dot", *xnode);
        xnode->toBinary("xnode.dat");

        {
            Source src( Module::OpenFile("xnode.dat") );
            AutoPtr<Syntax::XNode> reloaded = J.reload(src);
        }

        std::cerr << "Compiling..." << std::endl;
        JCompiler tr;
        tr.verbose = true;
        tr.policy  = Syntax::Permissive;
        tr(*xnode);

        std::cerr << "Walking..." << std::endl;
        xwalk(*xnode);
    }
}
Y_UDONE()

