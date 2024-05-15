#include "y/chemical/reactive/cluster.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(Equilibria        &eqs,
                          const Fragment    &fragment,
                          const Constants   &topK,
                          XMLog             &xml) :
        ClusterConstellation(eqs,fragment,topK,xml),
        limited(species.size,true),
        next(0),
        prev(0)
        {
            for(const SNode *node=unboundedSpecies.head;node;node=node->next)
            {
                const size_t i = (**node).indx[SubLevel];
                Coerce(limited[i]) = false;
            }

            for(const SNode *node=conservedSpecies.head;node;node=node->next)
            {
                const size_t i = (**node).indx[SubLevel];
                Coerce(limited[i]) = true;
            }

           // std::cerr << "limited=" << limited << std::endl;
            //exit(0);
            for(const SNode *node=species.head;node;node=node->next)
            {
                Y_XMLOG(xml, "limited " << **node << " = " << isLimited(**node));
            }
        }

        bool Cluster:: isLimited(const Species &sp) const noexcept
        {
            assert(species.has(sp));
            return limited[sp.indx[SubLevel]];
        }


        Cluster::  ~Cluster() noexcept {}


        void Cluster:: getK(const real_t t)
        {
            Writable<xreal_t> &K = *sharedK;
            for(const ENode *node=head;node;node=node->next)
            {
                Equilibrium &eq = Coerce(**node);
                K[eq.indx[TopLevel]] = eq.K(t);
            }
        }


        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            os << "<Cluster species='" << cl.species.size << "' eqs='" << cl.size << "'>" << std::endl;
            for(const ENode *node=cl.head;node;node=node->next)
            {
                cl.eqfmt.display(os << "  ", **node) << std::endl;
            }
            os << "  (*) " << cl.species << std::endl;
            return os << "<Cluster/>";
        }


        void Cluster:: viz(OutputStream &fp, const size_t order, const size_t clusterIndex) const
        {
            assert(order>0);
            assert(order<=blend.size());
            const EList &eqs = blend[order];
            fp("subgraph cluster_%lu {\n", static_cast<unsigned long>(clusterIndex));
            for(const SNode *sn = species.head;sn;sn=sn->next)
            {
                (**sn).viz(fp);
            }

            for(const ENode *en = eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.viz(fp,SubLevel);
                eq.vizLink(fp,SubLevel);
            }

            if(1==order)
            {
                for(const Conservation::Law *law=laws->head;law;law=law->next)
                {
                    law->viz(fp);
                }
            }

            fp << "}\n";
        }


        void Cluster:: moveControlled(XWritable       &target,
                                      const Level      tgtlvl,
                                      const Components &components,
                                      const xreal_t    cursor,
                                      const SNode     *zeroed,
                                      const XReadable &source,
                                      const Level      srclvl) const
        {
            static const char here[] = "Chemical::Cluster::moveControlled";
            const xreal_t     zero;
            const size_t      n = components->size();

            transfer(target,tgtlvl,source,srclvl);

            // pass 1: compute new set of concentrations
            {
                Components::ConstIterator it = components->begin();
                for(size_t i=n;i>0;--i,++it)
                {
                    const Component     &cm = *it;
                    const Species       &sp = cm.sp;
                    const size_t * const id = sp.indx;
                    const xreal_t        nu = cm.xn;
                    const xreal_t        dc = nu * cursor;
                    const xreal_t        c0 = source[ id[srclvl] ];
                    const xreal_t        c1 = c0+dc;
                    xreal_t             &cc = target[ id[tgtlvl] ];
                   
                    if( isLimited(sp) )
                    {
                        switch( Sign::Of(c0) )
                        {
                            case Negative:
                                if(dc<zero) throw Specific::Exception(here, "negative extent for negative [%s]", sp.name.c_str());
                                cc = Min(zero,c1);
                                break;

                            case __Zero__:
                                if(dc<zero) throw Specific::Exception(here, "negative extent for zero [%s]", sp.name.c_str());
                                assert(c1>=zero);
                                cc = c1;
                                break;

                            case Positive:
                                cc = Max(zero,c1);
                                break;
                        }
                    }
                    else
                    {
                        cc = c1;
                    }

                }
            }

            // pass 2: enforce zeroed
            for(;zeroed;zeroed=zeroed->next)
            {
                target[ (**zeroed).indx[tgtlvl] ] = zero;
            }

            // pass 3, compute gain
        }

    }

}
