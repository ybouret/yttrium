
#include "y/chemical/plexus/conservation/wardens.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Wardens:: ~Wardens() noexcept
            {
            }

            Y_PROXY_IMPL(Wardens, my)

            Wardens:: Wardens(const Clusters &clusters) :
            Proxy<const ListOf<Warden> >(),
            my(),
            cs(clusters)
            {
                for(const Cluster *cluster=cs->head;cluster;cluster=cluster->next)
                {
                    for(const Canon *canon = cluster->canons.head;canon;canon=canon->next)
                    {
                        my.pushTail( new Warden(*cluster,*canon) );
                    }
                }
            }

            void Wardens:: operator()(XMLog     &xml,
                                      XWritable &C0)
            {
                //cs.zset(I0);
                for(Warden *node=my.head;node;node=node->next)
                {
                    Warden &warden = *node;
                    warden(xml,C0,TopLevel);
                    /*
                    for(const SNode *sn=warden.canon.species->head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        sp(I0,TopLevel) = sp(warden.injected,AuxLevel).sum();
                    }
                     */

                }
            }
        }

    }

}
