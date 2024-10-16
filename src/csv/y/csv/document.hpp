//! \file

#ifndef Y_CSV_Document_Included
#define Y_CSV_Document_Included 1

#include "y/csv/line.hpp"
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
    }

}

#endif

