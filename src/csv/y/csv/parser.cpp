#include "y/csv/parser.hpp"
#include "y/jive/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace CSV
    {
        class Parser :: Code : public Jive::Parser
        {
        public:

            inline explicit Code() : Jive::Parser("CSV")
            {
                Agg        & CSV   = agg("CSV");
                const Rule & JSTR  = plug<Jive::Lexical::JString>("JString");
                const Rule & RSTR  = plug<Jive::Lexical::RString>("RString");
                const Rule & DATA  = term("DATA","[[:alnum:][:blank:]]+");
                const Rule & FIELD = alt("FIELD") << DATA << JSTR << RSTR;
                const Rule & COMMA = term("COMMA",',');
                const Rule & ENDL  = endl("ENDL","[:endl:]",false);
                const Rule & LINE  = agg("LINE") << zom(pick(FIELD,COMMA)) << ENDL;

                CSV << zom(LINE);

                lexer.drop("[:blank:]");
                renderGraphViz();
                validate();
            }

            inline virtual ~Code() noexcept {}


            inline Document * docFrom(Jive::Module *m)
            {
                const AutoPtr<XNode> ast = (*this)(m); checkAST(ast);
                const XList         &xll = ast->branch();
                AutoPtr<Document>    doc = new Document(xll.size);

                for(const XNode *node=xll.head;node;node=node->next)
                {
                    assert(Jive::Syntax::IsInternal == node->type);
                    assert("LINE" == node->name() );

                    AddLineTo(*doc,node);
                }

                return doc.yield();
            }
            
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


            static inline void AddLineTo(Document    &       doc,
                                         const XNode * const node)
            {
                std::cerr << "Parsing line" << std::endl;
                const XList &line = node->branch();
                for(const XNode *curr=line.head;curr;curr=curr->next)
                {
                    std::cerr << curr->name() << std::endl;
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
