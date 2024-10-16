
//! \file

#ifndef Y_CSV_Line_Included
#define Y_CSV_Line_Included 1

#include "y/csv/field.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{

    namespace CSV
    {

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
    }

}

#endif

