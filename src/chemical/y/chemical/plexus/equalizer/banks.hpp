
//! \file


#ifndef Y_Chemical_Banks_Included
#define Y_Chemical_Banks_Included 1

#include "y/chemical/plexus/equalizer/cursor.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class EqzBanks
        {
        public:
            explicit EqzBanks();
            virtual ~EqzBanks() noexcept;

            SBank  sb;
            CrBank cb;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqzBanks);
        };

     
    }

}

#endif

