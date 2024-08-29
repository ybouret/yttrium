
//! \file

#ifndef Y_Chemical_Warden_Fixed_Included
#define Y_Chemical_Warden_Fixed_Included 1

#include "y/chemical/plexus/cluster/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Concentrations that fix a broken law
        //
        //
        //______________________________________________________________________
        class Fixed
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Fixed,XMemory> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            Fixed(XWritable &, const Conservation::Law &) noexcept; //!< initialize with no gain
            Fixed(const Fixed &) noexcept;                          //!< duplicate
            ~Fixed() noexcept;                                      //!< cleanup
            Y_OSTREAM_PROTO(Fixed);                                 //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compute if law is still broken
            bool still(const XReadable &C,
                       const Level      L,
                       XAdd            &xadd);

            //! comparison by decreasing gain
            static int Compare(const Fixed &lhs, const Fixed &rhs) noexcept;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t                 gg;  //!< gain
            XWritable &             cc;  //!< persistent fixed concentration
            const Conservation::Law &cl; //!< persistent conservation law

        private:
            Y_DISABLE_ASSIGN(Fixed);
        };

    }

}

#endif
