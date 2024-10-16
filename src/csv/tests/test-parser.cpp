#include "y/csv/parser.hpp"
#include "y/csv/line.hpp"

#include "y/utest/run.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{

    namespace CSV
    {
       
        
        typedef Vector<Line,Memory::Dyadic> Lines;

        class Document : public Object, public Counted, public Lines
        {
        public:
            explicit Document() noexcept;
            virtual ~Document() noexcept;
            Document(const Document &);

            virtual const char * callSign() const noexcept;

        private:
            Y_DISABLE_ASSIGN(Document);

        };

        Document::  Document() noexcept : Object(), Counted(), Lines() {}
        Document:: ~Document() noexcept {}
        Document::  Document(const Document &doc) :
        Identifiable(),
        Collection(),
        Object(),
        Counted(),
        Lines(doc)
        {}

        const char * Document:: callSign() const noexcept
        {
            return "CSV::Document";
        }



    }
}

using namespace Yttrium;



Y_UTEST(parser)
{
    CSV::Parser parser;

    Y_SIZEOF(CSV::Field);
    Y_SIZEOF(CSV::Line);
    Y_SIZEOF(CSV::Document);

    if(argc>1)
    {
        AutoPtr<Jive::Syntax::XNode> tree = parser( Jive::Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng( *parser.name + "-tree.dot", *tree);

    }

}
Y_UDONE()
