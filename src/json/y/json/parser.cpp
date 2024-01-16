
#include "y/json/parser.hpp"
#include "y/jive/parser.hpp"
#include "y/type/nullify.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"

namespace Yttrium
{

    namespace JSON
    {
        const char * const Parser::CallSign = "JSON";

        namespace
        {

            class Code : public Jive::Parser
            {
            public:

                inline explicit Code() : Jive::Parser(JSON::Parser::CallSign)
                {

                    Alt        & G       = alt(name);

                    Alt        & VALUE   = alt("VALUE");
                    const Rule & COMMA   = mark(',');
                    const Rule & STRING  = plug<Jive::Lexical::JString>("STRING");
                    VALUE
                    << term("NUMBER","-?[:digit:]+")
                    << STRING
                    << term("true")
                    << term("false")
                    << term("null");
                    
                    
                    Alt & ARRAY = alt("ARRAY");
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




                    Alt & OBJECT = alt("OBJECT");
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

                    //renderGraphViz();
                    //validate();

                }

                inline virtual ~Code() noexcept
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            static void * CodeImpl[ Y_WORDS_FOR(Code) ];
            static Code * code = 0;
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

        
    }

}


