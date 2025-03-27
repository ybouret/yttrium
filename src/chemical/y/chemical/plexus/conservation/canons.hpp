
//! \file

#ifndef Y_Chemical_Conservation_Canons_Included
#define Y_Chemical_Conservation_Canons_Included 1

#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            class Canons : public CxxListOf<Canon>
            {
            public:
                explicit Canons(const ListOf<Law> &laws);
                virtual ~Canons() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canons);
            };

        }

    }

}

#endif

