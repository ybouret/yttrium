//! \file

#ifndef Y_CSV_Document_Included
#define Y_CSV_Document_Included 1

#include "y/csv/line.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{

    namespace CSV
    {
        //______________________________________________________________________
        //
        //
        //! alias to sequence of Lines
        //
        //______________________________________________________________________
        typedef Vector<Line,Memory::Dyadic> Lines;


        //______________________________________________________________________
        //
        //
        //
        //! a Document is a sequence of Lines
        //
        //
        //______________________________________________________________________
        class Document : public Object, public Counted, public Lines
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Document() noexcept; //!< setup empty
            virtual ~Document() noexcept; //!< cleanup
            Document(const Document &);   //!< hard copy

            //__________________________________________________________________
            //
            //
            //  Interface
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< [Identifiable] "CSV::Document"

        private:
            Y_DISABLE_ASSIGN(Document);

        };

        //______________________________________________________________________
        //
        //
        //! alias for a shared document
        //
        //______________________________________________________________________
        typedef ArcPtr<Document> SharedDocument;
    }

}

#endif

