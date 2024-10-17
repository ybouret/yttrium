#include "y/csv/parser.hpp"
#include "y/jive/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include "y/hashing/perfect.hpp"

namespace Yttrium
{

    namespace CSV
    {

        static const char *kws[] =
        {
            "JSTR",
            "RSTR",
            "DATA",
            "COMMA"
        };

#define Y_CSV_JSTR  0
#define Y_CSV_RSTR  1
#define Y_CSV_DATA  2
#define Y_CSV_COMMA 3



        static const size_t nkw = sizeof(kws)/sizeof(kws[0]);

        class Parser :: Code : public Jive::Parser
        {
        public:

            inline explicit Code() : Jive::Parser("CSV"), kw(kws,nkw)
            {
                Agg &        CSV      = agg("CSV");
                const Rule & MARK     = mark("MARK","[:blank:]");
                const Rule & MARKS    = zom(MARK);
                const Rule & JSTRING  = plug<Jive::Lexical::JString>("JSTRING");
                //const Rule & RSTRING  = plug<Jive::Lexical::RString>("RSTRING");
                const Rule & JSTR  = agg("JSTR") << MARKS << JSTRING << MARKS;
                //const Rule & RSTR  = agg("RSTR") << MARKS << RSTRING << MARKS;
                const Rule & ENDL  = endl("ENDL","[:endl:]",false);

                const Rule & LINE  = agg("LINE") << JSTR << ENDL;
                CSV << zom(LINE);

                renderGraphViz();

#if 0
                Agg        & CSV   = agg("CSV");
                const Rule & JSTR  = plug<Jive::Lexical::JString>("JSTR");
                const Rule & RSTR  = plug<Jive::Lexical::RString>("RSTR");
                const Rule & DATA  = term("DATA","[[:alnum:][:blank:]]+");
                const Rule & FIELD = alt("FIELD") << DATA << JSTR << RSTR;
                const Rule & COMMA = term("COMMA",',');
                const Rule & ENDL  = endl("ENDL","[:endl:]",false);
                const Rule & LINE  = agg("LINE") << zom(pick(FIELD,COMMA)) << ENDL;

                CSV << zom(LINE);

                lexer.drop("[:blank:]");
                renderGraphViz();
                validate();
#endif
            }

            inline virtual ~Code() noexcept {}


            inline Document * docFrom(Jive::Module *m)
            {

                const AutoPtr<XNode> ast = (*this)(m); checkAST(ast);
                const XList         &xll = ast->branch();
                AutoPtr<Document>    doc = new Document(xll.size);

#if 0
                for(const XNode *node=xll.head;node;node=node->next)
                {
                    assert(Jive::Syntax::IsInternal == node->type);
                    assert("LINE" == node->name() );

                    addLineTo(*doc,node);
                }
#endif

                return doc.yield();
            }

            const Hashing::Perfect kw;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);

            inline void checkAST(const AutoPtr<XNode> &ast)
            {

                if(ast.isEmpty())
                    throw Specific::Exception( name->c_str(), "invalid root");
                GraphViz::Vizible::DotToPng( *name + "-ast.dot", *ast);
                assert( "CSV" == ast->name() );
                assert( Jive::Syntax::IsInternal == ast->type);
            }


            inline void addLineTo(Document    &       doc,
                                  const XNode * const node) const
            {
                std::cerr << "Parsing line" << std::endl;
                Line & line = doc.add(); assert(doc.size()>0);
                try
                {
                    const XList &sub  = node->branch();
                    for(const XNode *curr=sub.head;curr;curr=curr->next)
                    {
                        const String       &cid = curr->name();
                        const Jive::Lexeme &lex = curr->lexeme();
                        std::cerr << cid << std::endl;
                        assert( Jive::Syntax::IsTerminal == curr->type );
                        switch( kw(cid) )
                        {
                            case Y_CSV_JSTR : {
                                std::cerr << "+" << lex << std::endl;
                                const String s = lex.toString(1,1);
                                const Field  f(s,Field::DQMarks);
                                line << f;
                            } break;

                            case Y_CSV_RSTR : {
                                std::cerr << "+" << lex << std::endl;
                                const String s = lex.toString(1,1);
                                const Field  f(s,Field::SQMarks);
                                line << f;
                            } break;

                            case Y_CSV_DATA : {
                                std::cerr << "+" << lex << std::endl;
                                const String s = lex.toString();
                                const Field  f(s,Field::Generic);
                                line << f;
                            }  break;

                            case Y_CSV_COMMA:
                                break;

                            default:
                                throw Specific::Exception( name->c_str(), "invalid content '%s'", cid.c_str() );
                        }
                    }
                    std::cerr << line << std::endl;
                }
                catch(...)
                {
                    assert(doc.size()>0);
                    doc.popTail();
                    throw;
                }
            }

        };

        Parser:: ~Parser() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Parser:: Parser() : code( new Code() )
        {

        }

        Document * Parser:: operator()(Jive::Module *m)
        {
            assert(0!=code);
            return code->docFrom(m);
        }
    }

}
