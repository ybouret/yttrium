#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Clusters,my)

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

                std::cerr << "new Cluster" << std::endl;
                my.pushTail( new Cluster(eq) );
                continue;

            CHECK_FUSION:
                {
                    Cluster::List ok;
                    while(my.size>0)
                    {
                        AutoPtr<Cluster> lhs = my.popHead();
                        for(Cluster *rhs=ok.head;rhs;rhs=rhs->next)
                        {
                            if(rhs->accepts(*lhs))
                            {
                                std::cerr << "Need To Implement Fusion!" << std::endl;
                                rhs->attach(*lhs);
                                lhs.erase();
                                break;
                            }
                        }

                        if(lhs.isValid()) ok.pushTail(lhs.yield());
                    }
                    my.swapWith(ok);
                }
            }
        }

    }

}

