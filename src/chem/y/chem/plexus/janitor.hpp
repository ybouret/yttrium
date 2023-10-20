
//! \file

#ifndef Y_Chemical_Janitor_Included
#define Y_Chemical_Janitor_Included 1

#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Conservation law
        //
        //
        //______________________________________________________________________
        class Janitor
        {
        public:
            explicit Janitor();
            virtual ~Janitor() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Janitor);
        };

    }

}

#endif

