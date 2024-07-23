
//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Phase space solution of a single equilibrium
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
            typedef CxxSeries<Prospect,Memory::Dyadic> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from solver persistent data and aftermath result
            Prospect(const Equilibrium &_eq,
                     const xreal_t      _eK,
                     const xreal_t      _xi,
                     const XReadable   &_cc,
                     const XReadable   &_dc,
                     XWritable         &_dd) noexcept;


            Prospect(const Prospect &) noexcept; //!< copy
            ~Prospect()                noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Prospect);           //!< display eq.name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! comparison by decreasing |xi|
            static int Compare(const Prospect &lhs,
                               const Prospect &rhs) noexcept;

            //! compute derivative and slope
            void   update(XAdd &xadd, XMul &xmul);
            size_t indx() const noexcept;

            //__________________________________________________________________
            //
            //
            // Member
            //
            //__________________________________________________________________

            const Equilibrium &eq; //!< used equilibrium
            const xreal_t      eK; //!< constant
            const xreal_t      xi; //!< its solving extent
            const xreal_t      ax; //!< |xi|
            const XReadable   &cc; //!< its solving phase space
            const XReadable   &dc; //!< cc-c0
            XWritable         &dd; //!< derivative
            const xreal_t      sl; //!< slope, initially 0
            
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        typedef Small::CoopLightList<Prospect> PList; //!< alias
        typedef PList::ProxyType               PBank; //!< alias
        typedef PList::NodeType                PNode; //!< alias
    }

}


#endif

