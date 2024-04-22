
//! \file

#ifndef Y_Chemical_Conservation_Groups_Included
#define Y_Chemical_Conservation_Groups_Included 1

#include "y/chemical/reactive/conservation/group.hpp"
#include "y/chemical/reactive/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            class Groups : public Proxy<const Group::List>
            {
            public:
                explicit Groups() noexcept;
                virtual ~Groups() noexcept;

                void collect(const Laws &laws);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Groups);
                Group::List cgl;

                virtual ConstInterface & surrogate() const noexcept;
                void fusion() noexcept;
            };


        }

    }

}

#endif
