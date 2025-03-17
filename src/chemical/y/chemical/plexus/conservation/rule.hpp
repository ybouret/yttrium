//! \file

#ifndef Y_Chemical_Conservation_Rule_Included
#define Y_Chemical_Conservation_Rule_Included 1

#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            class Rule : public Quantized, public Actors
            {
            public:
                explicit Rule() noexcept;
                virtual ~Rule() noexcept;

                Rule *next;
                Rule *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }

    }

}
#endif
