
//! \file


#ifndef Y_Chemical_Cursor_Included
#define Y_Chemical_Cursor_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/chemical/species.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! cursor at which (multiple) species vanish
        //
        //
        //______________________________________________________________________
        class Cursor : public SRepo
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cursor(const SBank   &sb,
                            const Species &sp,
                            const xreal_t  xx); //!< initialize
            Cursor(const Cursor &_);           //!< duplicate
            virtual ~Cursor() noexcept;        //!< cleanup
            Y_OSTREAM_PROTO(Cursor);           //!< display

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const xreal_t xi; //!< current extent

        private:
            Y_DISABLE_ASSIGN(Cursor);
        };



        typedef Small::CoopHeavyList<Cursor> CrList; //!< alias
        typedef CrList::NodeType             CrNode; //!< alias
        typedef CrList::ProxyType            CrBank; //!< alias

    }

}

#endif
