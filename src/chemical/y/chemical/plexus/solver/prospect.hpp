
//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! solution to a single running equilibrium
        //
        //
        //______________________________________________________________________
        class Prospect
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Prospect> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from aftermath
            Prospect(const Situation     _st,
                     const Equilibrium & _eq,
                     const xreal_t       _ek,
                     const XReadable &   _cc,
                     const xreal_t       _xi,
                     const XReadable &   _dc) noexcept;
            Prospect(const Prospect &_) noexcept; //!< duplicate
            ~Prospect() noexcept;                 //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! full human readable info
            std::ostream & show(std::ostream    &os,
                                const Cluster   &cl,
                                const XReadable * const Ktop) const;


            //! use equilibrium and its K to compute affinity
            xreal_t affinity(XMul            &X,
                             const XReadable &C,
                             const Level      L) const;

            //! compare by decreasing |xi|
            static int Compare(const Prospect &lhs, const Prospect &rhs) noexcept;

            //! append xi * nu to species's incresase
            void step(XSwell &inc) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Situation     st; //!< situation Running|Crucial
            const Equilibrium & eq; //!< the equlibrium
            const xreal_t       ek; //!< its precomputed constant
            const XReadable &   cc; //!< the solution
            const xreal_t       xi; //!< the extent from original state
            const XReadable &   dc; //!< cc-Corg
            const xreal_t       ax; //!< |xi|
            xreal_t             ff; //!< objective function value
            
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        typedef Small::CoopLightList<const Prospect> PRepo; //!< alias
        typedef PRepo::ProxyType                     PBank; //!< alias
        typedef PRepo::NodeType                      PNode; //!< alias

    }

}

#endif

