
//! \file

#ifndef Y_Chemical_Warden_Fund_Included
#define Y_Chemical_Warden_Fund_Included 1

#include "y/chemical/plexus/warden/frontier.hpp"
#include "y/chemical/plexus/cluster/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! multible banks for single argument constructors
        //
        //
        //______________________________________________________________________
        class Fund
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fund();          //!< setup
            virtual ~Fund() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SBank               sbank; //!< for species
            Conservation::LBank lbank; //!< for laws
            FBank               fbank; //!< for frontiers

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fund);
        };
    }

}

#endif

