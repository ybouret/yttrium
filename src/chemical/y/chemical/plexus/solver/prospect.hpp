//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium prospect
        //
        //
        //______________________________________________________________________
        class Prospect
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
         
            //! setup with all info
            Prospect(const Equilibrium & _eq,
                     const xreal_t       _eK,
                     const XReadable    &_cc,
                     const xreal_t       _xi,
                     const xreal_t       _ks) noexcept;

            Prospect(const Prospect &_) noexcept; //!< full copy
            ~Prospect()                 noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! return eq.indx[SubLevel]
            size_t sub() const noexcept;

            //! |ks*massAction(C)|
            xreal_t objectiveFunction(const XReadable &C,
                                      const Level      L,
                                      XMul &           X) const;

            //! compare by increasing |xi|
            static int Compare(const Prospect &lhs, const Prospect &rhs) noexcept
            {
                return Comparison::Increasing(lhs.ax,rhs.ax);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium &eq; //!< equilibrium
            const xreal_t      eK; //!< pre-computed constant
            const XReadable   &cc; //!< zero mass action here
            const xreal_t      xi; //!< evaluation of extent from Ctop
            const xreal_t      ax; //!< |xi|
            const xreal_t      ks; //!< scaling factor

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopLightList<const Prospect> PList; //!< alias
        typedef PList::NodeType                      PNode; //!< alias
        typedef PList::ProxyType                     PBank; //!< alias
    }

}

#endif

