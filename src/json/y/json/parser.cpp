
#include "y/json/parser.hpp"
#include "y/jive/parser.hpp"
#include "y/type/nullify.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/syntax/translator.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace JSON
    {
        const char * const Parser::CallSign = "JSON";

        namespace
        {

            class Code : public Jive::Parser, public Jive::Syntax::Translator
            {
            public:

                inline explicit Code() :
                Jive::Parser(JSON::Parser::CallSign),
                values(),
                pairs()
                {
                    setupParser();
                    setupLinker();
                }

                inline virtual ~Code() noexcept
                {
                }

                virtual void initialize()
                {
                    std::cerr << CallSign << " initialize..." << std::endl;
                    values.free();
                    pairs.free();
                }

                void run(Jive::Module *m)
                {
                    AutoPtr<XNode> ast = (*this)(m);
                    GraphViz::Vizible::DotToPng( *name + "-ast.dot", *ast);

                    //translate(*ast,Yttrium::Jive::Syntax::Permissive);
                    translate(*ast,Yttrium::Jive::Syntax::Restricted);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
                Vector<Value>      values;
                Vector<SharedPair> pairs;

                void setupParser();
                void setupLinker();

                Value &newValue()
                {
                    const Value tmp;
                    values << tmp;
                    return values.tail();
                }

                void onNumber(const Jive::Token &t)
                {
                    Jive::Syntax::Analyzer::onTerminal("Number", t);
                    const String s = t.toString();
                    Value        v(ASCII::Convert::ToReal<double>(s,"JSON::Number"));
                    newValue().swapWith(v);
                }

                void onnull(const Jive::Token &)
                {
                    const Value v;
                    values << v;
                }

                void ontrue(const Jive::Token &)
                {
                    const Value v(true);
                    values << v;
                }

                void onfalse(const Jive::Token &)
                {
                    const Value v(false);
                    values << v;
                }

                void onString(const Jive::Token &t)
                {
                    const String s = t.toString(1,1);
                    Value  v(s);
                    newValue().swapWith(v);
                }

                void onEmptyArray(const size_t)
                {
                    Value v(AsArray);
                    newValue().swapWith(v);
                }


                void onHeavyArray(const size_t n)
                {
                    Value v(AsArray);
                    Array &arr = v.as<Array>();
                    arr.reserve(n);

                    std::cerr << "values:" << std::endl << values << std::endl;
                    const size_t m = values.size();
                    for(size_t i=m-n+1;i<=m;++i)
                    {
                        arr.add(values[i]);
                    }
                    for(size_t j=n;j>0;--j) values.popTail();
                    newValue().swapWith(v);
                }

                void onEmptyObject(const size_t )
                {
                    Value v(AsObject);
                    newValue().swapWith(v);
                }

                void onPair(const size_t n)
                {
                    std::cerr << "onPair/" << n << std::endl;
                    std::cerr << "values: " << values << std::endl;
                    const size_t  m = values.size();
                    SharedPair    p = new Pair(values[m-1].as<String>());
                    p->v.swapWith(values.tail());
                    values.popTail();
                    values.popTail();
                    pairs << p;
                }

                void onHeavyObject(const size_t n)
                {
                    Value        v(AsObject);
                    JSON::Object &o = v.as<JSON::Object>();
                    const size_t  m = pairs.size();
                    for(size_t i=m+1-n;i<=m;++i)
                    {
                        const SharedPair &p = pairs[i];
                        if(!o.insert(p))
                            throw Specific::Exception("JSON::Object","multiple entry '%s'", p->k.c_str());
                    }
                }

            };

            static void * CodeImpl[ Y_WORDS_FOR(Code) ];
            static Code * code = 0;


            void Code:: setupParser()
            {
                Alt        & G       = alt(name);

                Alt        & VALUE   = alt("Value");
                const Rule & COMMA   = mark(',');
                const Rule & STRING  = plug<Jive::Lexical::JString>("String");
                VALUE
                << term("Number","-?[:digit:]+")
                << STRING
                << term("true")
                << term("false")
                << term("null");


                Alt & ARRAY = alt("Array");
                {
                    const Rule & LBRACK  = mark("[", "\\[");
                    const Rule & RBRACK  = mark("]","\\]");
                    ARRAY << ( agg("EmptyArray") << LBRACK << RBRACK);
                    {
                        Agg &HeavyArray = agg("HeavyArray");
                        HeavyArray << LBRACK;
                        HeavyArray << VALUE;
                        HeavyArray << zom( cat(COMMA,VALUE) );
                        HeavyArray << RBRACK;
                        ARRAY << HeavyArray;
                    }
                    VALUE << ARRAY;
                }
                G << ARRAY;




                Alt & OBJECT = alt("Object");
                {
                    const Rule & LBRACE = mark('{',"\\{");
                    const Rule & RBRACE = mark('}',"\\}");
                    OBJECT |= ( agg("EmptyObject") << LBRACE << RBRACE);
                    {
                        Agg &HeavyObject = agg("HeavyObject");

                        HeavyObject += LBRACE;
                        const Rule &Pair = agg("Pair") << STRING << mark(':') << VALUE;
                        HeavyObject += Pair;
                        HeavyObject += zom( cat(COMMA,Pair) );
                        HeavyObject += RBRACE;

                        OBJECT |= HeavyObject;
                    }
                    VALUE << OBJECT;
                }

                G << OBJECT;

                lexer.drop("[:blank:]");
                lexer.endl("[:endl:]");

                validate();
            }

            void Code:: setupLinker()
            {
                Y_Jive_OnTerminal(Code,Number);
                Y_Jive_OnTerminal(Code,null);
                Y_Jive_OnTerminal(Code,true);
                Y_Jive_OnTerminal(Code,false);
                Y_Jive_OnTerminal(Code,String);

                Y_Jive_OnInternal(Code,EmptyArray);
                Y_Jive_OnInternal(Code,HeavyArray);
                Y_Jive_OnInternal(Code,EmptyObject);
                Y_Jive_OnInternal(Code,Pair);
                Y_Jive_OnInternal(Code,HeavyObject);
            }
        }

        Parser:: ~Parser() noexcept
        {
            assert(0!=code);
            (void) Memory::OutOfReach::Naught(code);
            code = 0;
        }

        Parser:: Parser() : Singleton<Parser>()
        {
            code = new (Y_STATIC_ZARR(CodeImpl)) Code();
        }

        void Parser:: renderGraphViz() const
        {
            assert(0!=code);
            code->renderGraphViz();
        }

        void Parser:: run(Jive::Module *m)
        {
            assert(0!=code);
            code->run(m);
        }

        
    }

}


