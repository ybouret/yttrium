
#include "y/chemical/plexus/reactors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Reactors:: Reactors(const Clusters &cls) : Proxy<const Reactor::List>(), my()
        {
            for(const Cluster *cl=cls->head;cl;cl=cl->next) {
                my.pushTail( new Reactor(*cl) );
            }
        }


        Reactors:: ~Reactors() noexcept
        {

        }

        Y_PROXY_IMPL(Reactors,my)
    }

}
