
#include "y/chem/plexus.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        void Plexus:: buildClusters(Equilibria &eqs, XMLog &xml)
        {
            Y_XML_SECTION(xml, "BuildClusters");
            assert(0==clusters.size);

            //------------------------------------------------------------------
            //
            // loop over each equilibrium
            //
            //------------------------------------------------------------------
            for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
            {
                Equilibrium &eq = **it;
                bool         accepted = false;
                for(Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
                {
                    if(cluster->tiedTo(eq))
                    {
                        cluster->enroll(eq);
                        accepted = true;
                        break;
                    }
                }

                if(accepted)
                {
                    //----------------------------------------------------------
                    // check fusion
                    //----------------------------------------------------------
                    Y_XMLOG(xml," (add) <" << eq.name << ">");
                    Clusters temp;
                    while(clusters.size>0)
                    {
                        AutoPtr<Cluster> lhs = clusters.popHead();
                        for(Cluster *rhs=temp.head;rhs;rhs=rhs->next)
                        {
                            if(rhs->tiedTo(*lhs))
                            {
                                Y_XMLOG(xml, " (-^-)");
                                Coerce(rhs->all).mergeTail( Coerce(lhs->all) );
                                lhs = 0;
                                assert(lhs.isEmpty());
                                break;
                            }
                        }
                        if(lhs.isValid()) temp.pushTail( lhs.yield() );
                    }
                    clusters.swapWith(temp);
                }
                else
                {
                    //----------------------------------------------------------
                    // new cluster
                    //----------------------------------------------------------
                    Y_XMLOG(xml," (new) <" << eq.name << ">");
                    clusters.pushTail( new Cluster(eq) );
                }
            }

            Y_XMLOG(xml, "#cluster=" << clusters.size);
            for(Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
            {
                cluster->compile(eqs,shared_K,xml);
                Coerce(maxOrder) = Max(maxOrder,cluster->meg->size());
            }
            Y_XMLOG(xml, "maxOrder=" << maxOrder);
        }
    }

}

