//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium/outcome.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibirum prospect
        //
        //
        //______________________________________________________________________
        class Prospect : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit Prospect(const Outcome &   _out,
                              const xReal       _xi,
                              const XReadable & _dx) noexcept;

            //! duplicate
            explicit Prospect(const Prospect &) noexcept;

            //! cleanup
            virtual ~Prospect() noexcept;


            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept; //! [Entity] out.eq.name

            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            xReal score(XMul &X, const XReadable &C, const Level &L) const;
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Outcome     out;   //!< current outcome
            const xReal       xi;    //!< current extent to outcome
            const xReal       ax;    //!< |xi|
            const XReadable & dc;    //!< SubLevel deltaC
            xReal             ff;    //!< score

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopHeavyList<Prospect> ProList; //!< alias
        typedef ProList::NodeType              ProNode; //!< alias
        typedef ProList::ProxyType             ProBank; //!< alias

    }

}

#endif
