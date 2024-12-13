
//! \file

#ifndef Y_Chemical_Conservation_Conducts_Included
#define Y_Chemical_Conservation_Conducts_Included 1

#include "y/chemical/plexus/conservation/conduct.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            class Laws;

            class Conducts : public Proxy<const Act::List>
            {
            public:
                explicit Conducts(const Laws &laws);
                virtual ~Conducts() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Conducts);
                virtual ConstInterface & surrogate() const noexcept;
                void                     reconnect();
                Act::List my;
            };
        }

    }

}

#endif
