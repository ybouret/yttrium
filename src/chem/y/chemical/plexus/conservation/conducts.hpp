
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

            class Acts : public Proxy<const Act::List>
            {
            public:
                explicit Acts(const Laws &laws);
                virtual ~Acts() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Acts);
                virtual ConstInterface & surrogate() const noexcept;
                void                     reconnect();
                Act::List my;
            };
        }

    }

}

#endif
