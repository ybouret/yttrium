
//! \file

#ifndef Y_CSV_Line_Included
#define Y_CSV_Line_Included 1

#include "y/csv/field.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //! alias to sequence of Fields
        //
        //______________________________________________________________________
        typedef Vector<Field> Fields;

        //______________________________________________________________________
        //
        //
        //
        //! a Line is a sequence of Fields
        //
        //
        //______________________________________________________________________
        class Line :  public Fields
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Line() noexcept;          //!< setup empty
            explicit Line(const size_t capa);  //!< setup with capacity
            virtual ~Line() noexcept;          //!< cleanup
            Line(const Line &);                //!< copy

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< [Identifiable] "CSV::Line"

        private:
            Y_DISABLE_ASSIGN(Line);
        };
    }

}

#endif

