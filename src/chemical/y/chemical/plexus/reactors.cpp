
#include "y/chemical/plexus/reactors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Reactors:: Reactors(const Clusters &cls) :
        Proxy<const Reactor::List>(),
        my(),
        clusters(cls)
        {
            for(const Cluster *cl=clusters->head;cl;cl=cl->next) {
                my.pushTail( new Reactor(*cl) );
            }
        }


        Reactors:: ~Reactors() noexcept
        {

        }

        void Reactors:: operator()(XMLog &xml, XWritable &C0, Reactor::Proc * const cb)
        {
            Y_XML_SECTION_OPT(xml,"Reactors","count=" << my.size);

            for(Reactor *p=my.head;p;p=p->next)
            {
                p->mix(xml,C0,clusters.K,cb);
            }
        }


        Y_PROXY_IMPL(Reactors,my)
    }

}
