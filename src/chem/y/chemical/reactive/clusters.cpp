
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Clusters:: CallSign = "Chemical::Clusters";

        Clusters:: ConstInterface & Clusters:: surrogate() const noexcept
        {
            return cls;
        }

        Clusters:: Clusters(const Equilibria &eqs,
                            XMLog            &xml) :
        Proxy<const Cluster::List>(),
        cls()
        {
            Y_XML_SECTION(xml,CallSign);

            //------------------------------------------------------------------
            //
            // build grouping
            //
            //------------------------------------------------------------------
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                bool               accepted = false;
                for(Cluster *cl=cls.head;cl;cl=cl->next)
                {
                    if(cl->accepts(eq))
                    {
                        cl->addPrimary(eq);
                        accepted = true;
                        checkFusion();
                        break;
                    }
                }
                if(!accepted) cls.pushTail( new Cluster(eq) );

            }

            //------------------------------------------------------------------
            //
            // compile data
            //
            //------------------------------------------------------------------
            unsigned indx=0;
            for(Cluster *cl=cls.head;cl;cl=cl->next)
            {
                Coerce(cl->indx) = ++indx;
                cl->compile(xml);
            }

        }

        Clusters:: ~Clusters() noexcept
        {
        }


        

        void Clusters:: checkFusion() noexcept
        {
            Cluster::List store;
            while(cls.size>0)
            {
                AutoPtr<Cluster> rhs = cls.popHead();
                for(Cluster *lhs=store.head;lhs;lhs=lhs->next)
                {
                    if(lhs->accepts(*rhs))
                    {
                        lhs->addPrimary(rhs.yield());
                        break;
                    }
                }

                if(rhs.isValid())
                    store.pushTail( rhs.yield() ); // untouched
            }
            cls.swapWith(store);
        }


        std::ostream & operator<<(std::ostream &os, const Clusters &cls)
        {
            if(cls->size<=0)
                os << "{}";
            else
            {
                os << '{' << std::endl;
                for(const Cluster *cl=cls->head;cl;cl=cl->next)
                {
                    std::cerr << *cl << std::endl;
                }
                os << '}';
            }
            return os;
        }


    }
    
}
