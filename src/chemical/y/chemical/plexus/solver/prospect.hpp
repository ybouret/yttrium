
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
                     XWritable &         _cc,
                     const xreal_t       _xi,
                     XWritable &         _dc) noexcept;
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
            static SignType CompareDecreasingAX(const Prospect &lhs, const Prospect &rhs) noexcept;

            //! compare by increasing ff if ok, eq.indx if not ok
            static SignType CompareIncreasingFF(const Prospect &lhs, const Prospect &rhs) noexcept;


            void   step(XSwell &inc) const; //! append xi * nu to species's incresase
            String fileName()        const; //!< [good_|bad_] + eq.fileName() + ".pro"


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Situation     st; //!< situation Running|Crucial
            const Equilibrium & eq; //!< the equlibrium
            const xreal_t       ek; //!< its precomputed constant
            XWritable &         cc; //!< the solution
            xreal_t             xi; //!< the extent from original state
            xreal_t             ax; //!< |xi|
            XWritable &         dc; //!< cc-Corg
            xreal_t             ff; //!< objective function value
            bool                ok; //!< contributes to ff decrease
            
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        typedef Small::CoopLightList<const Prospect> PRepo; //!< alias
        typedef PRepo::ProxyType                     PBank; //!< alias
        typedef PRepo::NodeType                      PNode; //!< alias

    }

}

#endif

