
//! \file

#ifndef Y_CSV_Field_Included
#define Y_CSV_Field_Included 1

#include "y/string.hpp"

namespace Yttrium
{

    namespace CSV
    {
        //______________________________________________________________________
        //
        //
        //
        //! field=value of CSV
        //
        //
        //______________________________________________________________________
        class Field
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________

            //! format kind
            enum Kind
            {
                Generic, //!< whatever
                DQMarks, //!< double quotation marks
                SQMarks  //!< single quotation marks
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Field();               //!< setup empty, generic
            ~Field() noexcept;     //!< cleanup
            Field(const Field &);  //!< copy

            //! setup with some data and kind
            template <typename DATA> inline
            Field(const DATA &d, const Kind k) : text(d), kind(k) {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            String text; //!< content
            Kind   kind; //!< format
            
        private:
            Y_DISABLE_ASSIGN(Field);
        };

    }

}

#endif

