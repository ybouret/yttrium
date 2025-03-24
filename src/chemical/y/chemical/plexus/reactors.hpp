
//! \file


#ifndef Y_Chemical_Reactors_Included
#define Y_Chemical_Reactors_Included 1

#include "y/chemical/plexus/reactor.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Reactors : public Proxy<const Reactor::List>
        {
        public:
            explicit Reactors(const Clusters &);
            virtual ~Reactors() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactors);
            Y_PROXY_DECL();
            Reactor::List my;

        };
    }

}

#endif
