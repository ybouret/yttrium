
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chemical/reactive/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {
            class Canon : public Law::List, public Assembly
            {
            public:
                explicit Canon() noexcept;
                virtual ~Canon() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canon);
            };
        }

    }

}

#endif

