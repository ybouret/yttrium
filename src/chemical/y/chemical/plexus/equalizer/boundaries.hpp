//! \file

#ifndef Y_Chemical_Boundaries_Included
#define Y_Chemical_Boundaries_Included 1

#include "y/chemical/plexus/equalizer/boundary.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::CoopHeavyList<Boundary> BList;
        typedef BList::NodeType                BNode;
        typedef BList::ProxyType               BBank;
        
        class Banks
        {
        public:
            Banks();
            ~Banks() noexcept;

            BBank b;
            SBank s;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Banks);
        };

        //______________________________________________________________________
        //
        //
        //
        //! ordered boundaries
        //
        //
        //______________________________________________________________________
        class Boundaries : public BList
        {
        public:
            explicit Boundaries(const Banks &) noexcept;
            virtual ~Boundaries() noexcept;

            //! insert vanishing extent and its species
            void operator()(const xreal_t   xi,
                            const Species & sp);

            //! check sorted
            bool sorted() const noexcept;
            

        private:
            const SBank sbank;
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
            void atTail(const xreal_t xi, const Species &sp);
            void atHead(const xreal_t xi, const Species &sp);
        };

    }

}

#endif

