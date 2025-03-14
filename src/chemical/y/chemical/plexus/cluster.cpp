
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Cluster::CallSign = "Cluster";

        Y_PROXY_IMPL(Cluster,my)

        Cluster:: ~Cluster() noexcept
        {
            
        }

        Cluster:: Cluster(Equilibrium &first) :
        Proxy<const ClusterType>(),
        my(),
        next(0),
        prev(0)
        {
            my.attach(first);
        }

        void Cluster:: attach(Equilibrium &eq)
        {
            if(latched)                 throw Specific::Exception(CallSign, "latched while attaching '%s'", eq.key().c_str());
            if(my.equilibria->has(eq))  throw Specific::Exception(CallSign, "attaching multiple '%s'", eq.key().c_str());
            my.attach(eq);
        }

        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            return os << cl->equilibria << '/' << cl->species;
        }


        bool Cluster:: accepts(const Equilibrium &eq) const noexcept
        {
            for(const SNode *sn=my.species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(eq->search(sp.key())) return true;
            }
            return false;
        }

        bool Cluster:: accepts(const Cluster &cluster) const noexcept
        {
            for(const SNode *sn=my.species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(cluster->species->has(sp)) return true;
            }
            return false;
        }

        void Cluster:: attach(Cluster &cluster)  
        {
            if(latched || cluster.latched) throw Specific::Exception(CallSign,"cannot attached latched cluster(s)");
            my.fusion(cluster.my);
        }


        void Cluster:: compile(XMLog &xml)
        {
            Y_XML_SECTION_OPT(xml, CallSign,*this);
            const size_t N = my.equilibria->size; assert(N>0);
            const size_t M = my.species->size;    assert(M>0);
            iMatrix &topo = Coerce(topology);
            topo.make(N,M);
            for(const ENode *en=my.equilibria->head;en;en=en->next)
            {
                const Components &eq = **en;
                (**en).fillTopology(topo[eq.indx[SubLevel]],SubLevel);
            }
            Y_XMLOG(xml, "topology=" << topology);
        }



    }

}


