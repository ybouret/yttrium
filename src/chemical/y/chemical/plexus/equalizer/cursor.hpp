
//! \file


#ifndef Y_Chemical_Cursor_Included
#define Y_Chemical_Cursor_Included 1

#include "y/chemical/plexus/equalizer/hauler.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            //__________________________________________________________________
            //
            //
            //
            //! cursor at which (multiple) species vanish
            //
            //
            //__________________________________________________________________
            class Cursor : public Hauler
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Cursor(const SBank   &sb,
                                const Species &sp,
                                const xreal_t  xx); //!< initialize
                Cursor(const Cursor &_);           //!< duplicate
                virtual ~Cursor() noexcept;        //!< cleanup
                Y_OSTREAM_PROTO(Cursor);           //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                virtual xreal_t getXi() const noexcept; //!< return xi

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xreal_t xi; //!< current extent

            private:
                Y_DISABLE_ASSIGN(Cursor);
            };



            typedef Small::CoopHeavyList<Cursor> CrList; //!< alias
            typedef CrList::NodeType             CrNode; //!< alias
            typedef CrList::ProxyType            CrBank; //!< alias

        }
    }

}

#endif
