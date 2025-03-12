

#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Clusters:: ~Clusters() noexcept
        {
            
        }

        Clusters:: Clusters(Equilibria &eqs) :
        my()
        {
            for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
            {
                Equilibrium &eq = **it;
                for(Cluster *cl=my.head;cl;cl=cl->next)
                {
                    if(cl->accepts(eq))
                    {
                        cl->attach(eq);
                        goto CHECK_FUSION;
                    }
                }

                my.pushTail( new Cluster(eq) );

                continue;
            CHECK_FUSION:
                {
                    Cluster::List ok;
                    
                }
            }
        }

    }

}
