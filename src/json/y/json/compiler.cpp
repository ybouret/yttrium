#include "y/json/compiler.hpp"
#include "y/type/nullify.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"
#include "y/lingo/parser.hpp"
#include "y/lingo/syntax/translator.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/oversized.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace JSON
    {

        using namespace Lingo;

        namespace
        {
            class JParser : public  Parser
            {
            public:
                explicit JParser() : Parser("JSON")
                {
                    Alt        & JSON   = alt(name);
                    Alt        & VALUE  = alt("Value");
                    const Rule & STRING = plug<Lexical::JString>("String");
                    const Rule & COMMA  = mark(',');
                    VALUE << STRING << term("Number", "[-+]?[:digit:]+([.][:digit:]+)?([eEdD][-+]?[:digit:]+)?");

                    Alt & ARRAY = alt("Array");
                    {
                        Agg        & HeavyArray = agg("HeavyArray");
                        HeavyArray << '[';
                        HeavyArray << VALUE;
                        HeavyArray << zom( cat(COMMA,VALUE) );
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
                        HeavyObject << zom( cat(COMMA,PAIR) );
                        HeavyObject << '}';

                        OBJECT << HeavyObject;
                        OBJECT << (agg("EmptyObject") << '{' << '}');
                    }
                    VALUE << OBJECT;
                    VALUE <<  "true" << "false" << "null";

                    // finish top-level
                    JSON << ARRAY;
                    JSON << OBJECT;


                    lexer.drop("[:blank:]");
                    lexer.endl("[:endl:]", Lexeme::Drop);

                    render();
                    validate();
                    
                }

                virtual ~JParser() noexcept
                {

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JParser);
            };

        }

        namespace
        {
#define Y_JSON_Terminal(NAME) Y_Lingo_OnTerminal(JLinker,NAME)
#define Y_JSON_Internal(NAME) Y_Lingo_OnInternal(JLinker,NAME)

            class JLinker : public Syntax::Translator
            {
            public:


                explicit JLinker(const Caption &label) :
                Syntax::Translator(label),
                values(),
                pairs()
                {
                    Y_JSON_Terminal(String);
                    Y_JSON_Terminal(Number);
                    Y_JSON_Terminal(null);
                    Y_JSON_Terminal(true);
                    Y_JSON_Terminal(false);

                    Y_JSON_Internal(EmptyArray);
                    Y_JSON_Internal(HeavyArray);

                    Y_JSON_Internal(EmptyObject);
                    Y_JSON_Internal(Pair);

                }

                virtual ~JLinker() noexcept
                {

                }

                virtual void init() noexcept
                {
                    values.free();
                    pairs.free();
                }



                Vector<Value>      values;
                Vector<SharedPair> pairs;


                void onString(const Lexeme &lx)
                {
                    const String s = lx.toString(1,1);
                    const Value  v= s;
                    values << v;

                }

                void onnull(const Lexeme &)  { const Value _;        values << _; }
                void ontrue(const Lexeme &)  { const Value _(true);  values << _; }
                void onfalse(const Lexeme &) { const Value _(false); values << _; }
                void onNumber(const Lexeme &lx)
                {
                    const String s = lx.toString();
                    const Number n = ASCII::Convert::ToReal<Number>(s,"JSON::Number");
                    const Value  _ = n;
                    values << _;
                }

                void onEmptyArray(const size_t)  { const Value _(AsArray);  values << _; }
                void onEmptyObject(const size_t) { const Value _(AsObject); values << _; }

                void onHeavyArray(const size_t n)
                {
                    assert(n>0);
                    assert(n>=values.size());
                    const Value nil;
                    Value       v(AsArray);
                    Array &      a = v.as<Array>();
                    const size_t m = values.size();
                    for(size_t i=1+m-n;i<=m;++i)
                    {
                        a << nil;
                        a.tail().swapWith(values[i]);
                    }
                    for(size_t i=n;i>0;--i) values.popTail();
                    std::cerr << "a=" << a << std::endl;
                }

                void onPair(const size_t)
                {
                    assert(values.size()>=2);
                    Value        v; v.swapWith(values.tail());   values.popTail();
                    const String k = values.tail().as<String>(); values.popTail();
                    SharedPair   p = new Pair(k);
                    p->v.swapWith(v);
                    pairs << p;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JLinker);
            };
        }


        class Compiler :: Code : public Oversized
        {
        public:
            explicit Code() : parser(), linker(parser.name)
            {
            }


            virtual ~Code() noexcept
            {
            }

            JParser parser;
            JLinker linker;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        Compiler:: Compiler() :
        code( new Code() )
        {
            std:: cerr << "sizeof(Code) = " << sizeof(Code) << std::endl;
        }

        Compiler:: ~Compiler() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        void Compiler:: load(Value &jv, Lingo::Module * const jm)
        {
            jv.nullify();
            AutoPtr<Syntax::XNode> tree = code->parser(jm);
            GraphViz::Vizible::DotToPng("json-ast.dot", *tree);

            code->linker.verbose = true;
            //code->linker.policy  = Syntax::Permissive;
            code->linker( *tree );

        }

    }
}
