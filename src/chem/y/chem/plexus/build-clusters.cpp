
#include "y/chem/plexus.hpp"
#include "y/system/exception.hpp"

#include <iomanip>

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

            //------------------------------------------------------------------
            //
            // compile clusters and update information
            //
            //------------------------------------------------------------------
            Y_XMLOG(xml, "#cluster=" << clusters.size);
            for(Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
            {
                cluster->compile(eqs,shared_K,xml);
                const SpRepo &lib = cluster->lib;
                Coerce(maxOrder) = Max(maxOrder,cluster->meb->size());
                Coerce(maxTopId) = Max(maxTopId, (**(lib.tail)).indx[TopLevel] );
                Coerce(maxSubId) = Max(maxSubId, lib.size);
                Coerce(maxEqzSz) = Max(maxEqzSz,cluster->army->definite.size);
                assert(cluster->lib.size == (**(lib.tail)).indx[SubLevel]);
            }
            Y_XMLOG(xml, "maxOrder=" << maxOrder);
            Y_XMLOG(xml, "maxTopId=" << maxTopId);
            Y_XMLOG(xml, "maxSubId=" << maxSubId);
            Y_XMLOG(xml, "maxEqzSz=" << maxEqzSz);


            //------------------------------------------------------------------
            //
            // collect kept species per cluster
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Plexus::KeptSpecies");
                size_t kndx = 1;
                for(Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
                {
                    const Booleans &  kept = *(cluster->kept);
                    for(const SpNode *node = cluster->lib.head; node; node=node->next)
                    {
                        const Species &sp = **node;
                        if( !kept[sp.indx[SubLevel]]) continue;
                        Coerce(sp.indx[AuxLevel]) = kndx++;
                        Coerce(kSpecies) << sp;
                        Y_XMLOG(xml, "#" << std::setw(3) << sp.indx[AuxLevel] << " @" << sp);
                    }
                }
                SortIncreasing(Coerce(kSpecies));
            }

            if( (kSpecies.size>0 && maxEqzSz<=0) || (kSpecies.size<=0 && maxEqzSz>0) )
                throw Specific::Exception(CallSign,"corrupted kept species/definite equilibria");
            
        }
    }

}

