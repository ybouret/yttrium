#include "y/csv/parser.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{

    namespace CSV
    {
        class Field
        {
        public:
            enum Kind
            {
                Generic, //!< whatever
                DQMarks, //!< double quotation marks
                SQMarks  //!< single quotation marks
            };
            Field();
            ~Field() noexcept;
            Field(const Field &);

            template <typename DATA> inline
            Field(const DATA &d, const Kind k) : text(d), kind(k) {}


            String text;
            Kind   kind;
        private:
            Y_DISABLE_ASSIGN(Field);
        };

        Field:: Field() : text(), kind(Generic)
        {
        }

        Field:: ~Field() noexcept
        {
        }

        Field:: Field(const Field &other) : text(other.text), kind(other.kind) {}


        typedef Vector<Field> Fields;

        class Line :  public Fields
        {
        public:
            explicit Line() noexcept;
            explicit Line(const size_t capa);
            virtual ~Line() noexcept;
            Line(const Line &);

            virtual const char * callSign() const noexcept;

        private:
            Y_DISABLE_ASSIGN(Line);
        };


        Line:: ~Line() noexcept {}
        Line::  Line() noexcept : Fields() {}
        Line::  Line(const size_t capa) : Fields(capa,AsCapacity) {}
        Line::  Line(const Line &other) :
        Identifiable(), Collection(), Fields(other) {}

        const char * Line:: callSign() const noexcept
        {
            return "CSV::Line";
        }


    }
}

using namespace Yttrium;



Y_UTEST(parser)
{
    CSV::Parser parser;

    Y_SIZEOF(CSV::Field);
    Y_SIZEOF(CSV::Line);

    if(argc>1)
    {
        AutoPtr<Jive::Syntax::XNode> tree = parser( Jive::Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng( *parser.name + "-tree.dot", *tree);

    }

}
Y_UDONE()
