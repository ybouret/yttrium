
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Clusters:: ~Clusters() noexcept
        {
        }

        Clusters::ConstInterface & Clusters:: surrogate() const noexcept
        {
            return cls;
        }

        Clusters:: Clusters(Equilibria &eqs, XMLog &xml) :
        Proxy<const Cluster::List>(),
        cls(),
        shK()
        {
            Y_XML_SECTION(xml, "Clusters");
            for(Equilibria::ConstIterator it=eqs->begin(); it !=eqs->end(); ++it)
            {
                const Equilibrium &eq = **it;

                //--------------------------------------------------------------
                // look for an accepting cluster
                //--------------------------------------------------------------
                for(Cluster *cl=cls.head;cl;cl=cl->next)
                {
                    if(cl->accept(eq))
                    {
                        *cl << eq;
                        goto CHECK_FUSION;
                    }
                }

                // create new cluster
                cls.pushTail(new Cluster(eq) );
                continue;

                // check fusion
            CHECK_FUSION:
                Cluster::List store;
                while( cls.size > 0 )
                {
                    AutoPtr<Cluster> mine = cls.popHead();
                    for(Cluster *cl=store.head;cl;cl=cl->next)
                    {
                        if(cl->accept(*mine))
                        {
                            (*cl).mergeTail( *(mine.yield()) );
                            break;
                        }
                    }
                    if(mine.isValid()) store.pushTail( mine.yield() );
                }
                cls.swapWith(store);
            }

            size_t total = 0;
            for(Cluster *cl = cls.head; cl; cl=cl->next )
            {
                total += cl->compile(xml,eqs,shK).size;
            }
            shK.adjust(total,0);

        }


        const XReadable & Clusters:: K(const xreal_t t)
        {
            for(Cluster *cl = cls.head; cl; cl=cl->next )
            {
                for(ENode *en=cl->head;en;en=en->next)
                {
                    Equilibrium &eq = Coerce(**en);
                    shK[eq.indx[TopLevel]] = eq.K(t);
                }
            }
            return shK;
        }


        void Clusters:: show(std::ostream &os) const
        {
            for(const Cluster *cl = cls.head; cl; cl=cl->next )
            {
                cl->show(os);
            }
        }

        void Clusters:: showK(std::ostream &os) const
        {
            for(const Cluster *cl = cls.head; cl; cl=cl->next )
            {
                cl->show(os,shK);
            }
        }

    }

}