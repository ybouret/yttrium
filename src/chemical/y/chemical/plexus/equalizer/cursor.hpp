
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

        class Cursor : public SRepo
        {
        public:
            //! initialize
            explicit Cursor(const SBank   &sb,
                            const Species &sp,
                            const xreal_t  xx);
            Cursor(const Cursor &_);    //!< duplicate
            virtual ~Cursor() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Cursor);    //!< display


            const xreal_t xi; //!< current extent

        private:
            Y_DISABLE_ASSIGN(Cursor);
        };



        typedef Small::CoopHeavyList<Cursor> CrList;
        typedef CrList::NodeType             CrNode;
        typedef CrList::ProxyType            CrBank;

    }

}

#endif
