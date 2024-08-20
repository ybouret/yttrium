//! \file

#ifndef Y_Chemical_Boundaries_Included
#define Y_Chemical_Boundaries_Included 1

#include "y/chemical/plexus/equalizer/boundary.hpp"
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
        //! banks of BNode/SNode for Boundaries, Faders...
        //
        //______________________________________________________________________
        class Banks
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Banks();           //!< setup
            ~Banks() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            BBank b; //!< nodes for BList
            SBank s; //!< nodes for SRepo

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
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Boundaries(const Banks &) noexcept; //!< setup
            virtual ~Boundaries() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
          
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

