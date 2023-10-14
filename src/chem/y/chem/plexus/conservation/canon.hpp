
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chem/plexus/conservation.hpp"
#include "y/data/small/light/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::BareLightList<const Conservation> Conservations;


        class Canon : public Object, public Conservations
        {
        public:
            explicit Canon(const Conservation &first);
            virtual ~Canon() noexcept;

            Canon *next;
            Canon *prev;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Canon);
        };

    }

}

#endif

