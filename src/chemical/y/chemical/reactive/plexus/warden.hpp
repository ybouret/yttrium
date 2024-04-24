
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Warden
        {
        public:
            explicit Warden();
            virtual ~Warden() noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);
        };

    }

}

#endif
