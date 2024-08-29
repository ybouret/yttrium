//! \file

#ifndef Y_Chemical_Warden_Frontier_Included
#define Y_Chemical_Warden_Frontier_Included 1

#include "y/chemical/plexus/warden/sproxy.hpp"
#include "y/chemical/types.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! fixed value frontier, with one or more species
        //
        //
        //______________________________________________________________________
        class Frontier : public SProxy
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with first requirement
            explicit Frontier(const SBank &, const xreal_t, const Species &);
            explicit Frontier(const Frontier &); //!< duplicate
            virtual ~Frontier() noexcept;        //!< cleanup
            Y_OSTREAM_PROTO(Frontier);           //!< display


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Frontier & operator<<(const Species &); //!< wrapper to append a species

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const xreal_t xi; //!< required xi to reset species to 0
   
        private:
            Y_DISABLE_ASSIGN(Frontier);
        };

        typedef Small::CoopHeavyList<Frontier> FList; //!< alias
        typedef FList::ProxyType               FBank; //!< alias
        typedef FList::NodeType                FNode; //!< alias

    }

}

#endif

