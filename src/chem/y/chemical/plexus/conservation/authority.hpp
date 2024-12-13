

//! \file

#ifndef Y_Chemical_Conservation_Authority_Included
#define Y_Chemical_Conservation_Authority_Included 1

#include "y/chemical/plexus/conservation/conducts.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            class Authority : public Object
            {
            public:
                explicit Authority(Mix &, XMLog &);
                virtual ~Authority() noexcept;

                const Laws       laws;
                const Conducts   conducts;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Authority);
            };

        }

    }

}

#endif
