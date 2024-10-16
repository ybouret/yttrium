
//! \file

#ifndef Y_CSV_Field_Included
#define Y_CSV_Field_Included 1

#include "y/string.hpp"

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

    }

}

#endif

