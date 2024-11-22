#include "y/json/compiler.hpp"
#include "y/type/nullify.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"
#include "y/lingo/parser.hpp"
#include "y/lingo/syntax/translator.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/quantized.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace JSON
    {

        using namespace Lingo;

        namespace
        {
            class JParser : public Parser
            {
            public:
                explicit JParser() : Parser("JSON")
                {
                    Alt & JSON   = alt(name);
                    Alt & ARRAY  = alt("Array");
                    Alt & OBJECT = alt("Object");

                    JSON << ARRAY;
                    JSON << OBJECT;

                    {
                        Alt &        VALUE  = alt("Value");
                        const Rule & STRING = plug<Lexical::JString>("String"); // needed for Pair
                        //const Rule & COMMA  = mark(',');                        // needed for Array/Object

                        VALUE << STRING << term("Number", "[-+]?[:digit:]+([.][:digit:]+)?([eEdD][-+]?[:digit:]+)?");

                        {
                            Agg        & HeavyArray = agg("HeavyArray");
                            HeavyArray << '[';
                            HeavyArray << VALUE;
                            //HeavyArray << zom( cat(COMMA,VALUE) );
                            HeavyArray << extra(',',VALUE);
                            HeavyArray << ']';
                            ARRAY << HeavyArray;
                            ARRAY << (agg("EmptyArray") << '[' << ']');
                        }
                        VALUE << ARRAY;

                        {
                            Agg & HeavyObject = agg("HeavyObject");
                            HeavyObject << '{';
                            const Rule & PAIR = agg("Pair") << STRING << ':' << VALUE;
                            HeavyObject << PAIR;
                            //HeavyObject << zom( cat(COMMA,PAIR) );
                            HeavyObject << extra(',',PAIR);
                            HeavyObject << '}';

                            OBJECT << HeavyObject;
                            OBJECT << (agg("EmptyObject") << '{' << '}');
                        }
                        VALUE << OBJECT;
                        VALUE <<  "true" << "false" << "null";
                    }



                    lexer.drop("[:blank:]");
                    lexer.endl("[:endl:]",Drop);

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
                pairs(),
                nil()
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
                    Y_JSON_Internal(HeavyObject);

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
                    Value        v = s;
                    values << nil;
                    values.tail().swapWith(v);

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
                    assert(n<=values.size());
                    Value       v(AsArray);
                    {
                        Array &      a = v.as<Array>();
                        const size_t m = values.size();
                        for(size_t i=1+m-n;i<=m;++i)
                        {
                            a << nil;
                            a.tail().swapWith(values[i]);
                        }
                        for(size_t i=n;i>0;--i) values.popTail();
                    }
                    values << nil;
                    values.tail().swapWith(v);
                }

                void onPair(const size_t)
                {
                    assert(values.size()>=2);
                    Value        v;
                    v.swapWith(values.tail());
                    values.popTail();

                    const String k = values.tail().as<String>();
                    values.popTail();

                    SharedPair   p = new Pair(k);
                    p->v.swapWith(v);
                    pairs << p;
                }

                void onHeavyObject(const size_t n)
                {
                    assert(n>0);
                    assert(n<=pairs.size());

                    Value        v(AsObject);
                    {
                        Object &     o = v.as<Object>();
                        const size_t m = pairs.size();
                        for(size_t i=1+m-n;i<=m;++i)
                        {
                            const SharedPair &p =pairs[i];
                            if(!o.insert(p))
                                throw Specific::Exception(name->c_str(),"multiple key='%s",p.key().c_str());
                        }
                        for(size_t i=n;i>0;--i) pairs.popTail();
                    }
                    values << nil;
                    values.tail().swapWith(v);
                }

                const Value nil;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(JLinker);
            };
        }


        class Compiler :: Code : public Yttrium::Object
        {
        public:
            explicit Code() : parser( new JParser() ), linker( new JLinker(parser->name) )
            {
            }


            virtual ~Code() noexcept
            {
            }

            AutoPtr<JParser> parser;
            AutoPtr<JLinker> linker;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        Compiler:: Compiler() :
        code( new Code() ),
        verbose(code->linker->verbose)
        {

        }

        Compiler:: ~Compiler() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        void Compiler:: load(Value &jv, Lingo::Module * const jm)
        {
            jv.nullify();
            JParser &              parse = *(code->parser);
            JLinker &              link  = *(code->linker);
            AutoPtr<Syntax::XNode> xtree = parse(jm);
            GraphViz::Vizible::DotToPng("json-ast.dot", *xtree);

            //link.verbose = true;
            link( *xtree );
        }

    }
}
