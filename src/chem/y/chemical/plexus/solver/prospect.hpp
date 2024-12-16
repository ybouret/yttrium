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
        class Prospect : public Outcome
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
                              XWritable       & _dx) noexcept;

            //! duplicate
            explicit Prospect(const Prospect &) noexcept;

            //! cleanup
            virtual ~Prospect() noexcept;

            
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
            xReal             xi;    //!< current extent to outcome
            xReal             ax;    //!< |xi|
            XWritable       & dc;    //!< SubLevel deltaC
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
