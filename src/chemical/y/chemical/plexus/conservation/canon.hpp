
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chemical/plexus/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            typedef Small::BareLightList<const Law> LList;
            typedef LList::NodeType                 LNode;

            class Canon : public Object, public LList
            {
            public:
                explicit Canon(const Law &first);
                virtual ~Canon() noexcept;
                bool     accepts(const Law &) const noexcept; //!< if common actor

                Canon * next;
                Canon * prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canon);
            };

        }

    }

}

#endif

