
//! \file

#ifndef Y_Chemical_Warden_Trade_Included
#define Y_Chemical_Warden_Trade_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Concentrations that improve a negative value
        //
        //
        //______________________________________________________________________
        class Trade
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Trade,XMemory> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize
            Trade(const Equilibrium & _eq,
                  const XReadable &   _cc,
                  const xreal_t       _gg,
                  const XReadable &   _dc) noexcept;

            ~Trade()              noexcept; //!< cleanup
            Trade(const Trade &_) noexcept; //!< duplicate
            Y_OSTREAM_PROTO(Trade);         //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! comparison by decreasing gain
            static int Compare(const Trade &lhs, const Trade &rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium & eq; //!< persistent equilibria
            const XReadable   & cc; //!< improved concentrations
            const xreal_t       gg; //!< total gain
            const XReadable   & dc; //!< delta to compute multiple trades

        private:
            Y_DISABLE_ASSIGN(Trade);
        };

    }

}

#endif
