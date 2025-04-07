

//! \file

#ifndef Y_Chemical_Conservation_Broken_Included
#define Y_Chemical_Conservation_Broken_Included 1

#include "y/chemical/plexus/conservation/canon.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            //__________________________________________________________________
            //
            //
            //
            //! broken law and excess
            //
            //
            //__________________________________________________________________
            class Broken
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Broken(const xreal_t &_xs,
                       const Law     &_law) noexcept; //!< setup
                ~Broken()                   noexcept; //!< cleanup
                Broken(const Broken &)      noexcept; //!< duplicate

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! show with canon for format
                std::ostream & show(std::ostream &, const Assembly &) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xreal_t  xs; //!< excess for law
                const Law    &law; //!< the broken law

            private:
                Y_DISABLE_ASSIGN(Broken);
            };


            typedef Small::CoopHeavyList<Broken> BList; //!< alias
            typedef BList::NodeType              BNode; //!< alias
            typedef BList::ProxyType             BBank; //!< alias

        }

    }

}

#endif

