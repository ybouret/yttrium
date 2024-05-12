
//! \file

#ifndef Y_Chemical_Boundaries_Included
#define Y_Chemical_Boundaries_Included 1

#include "y/chemical/reactive/plexus/boundary.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::CoopHeavyList<Boundary> BList; //!< alias
        typedef BList::NodeType                BNode; //!< alias
        typedef BList::ProxyType               BBank; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! List of ordered boundaries
        //
        //
        //______________________________________________________________________
        class Boundaries : public Proxy<const BList>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with shared banks for allocation
            explicit Boundaries(const BBank &bbank,
                                const SBank &sbank) noexcept;
            virtual ~Boundaries() noexcept; //!< cleanup
            Boundaries(const Boundaries &); //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            bool contains(const Species &s) const noexcept; //!< mostly to debug
            bool validate()                 const noexcept; //!< mostly to debug
            void reset()                          noexcept; //!< free

            //! the extent make species disappear
            void operator()(const Species &s,
                            const xreal_t  x);

        private:
            Y_DISABLE_ASSIGN(Boundaries);
            BList       impl;
            const SBank repo;

            virtual ConstInterface & surrogate() const noexcept { return impl; }
        };

        
       


    }

}

#endif

