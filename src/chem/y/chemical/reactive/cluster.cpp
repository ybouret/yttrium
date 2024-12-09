
#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(const Equilibrium &first) :
        Proxy<const Grouping>(),
        my(),
        laws(0),
        conservancy(),
        conserved(),
        unbounded(),
        reacOnly(),
        prodOnly(),
        rigorous(),
        tolerant(),
        order(0),
        next(0),
        prev(0),
        indx(0),
        maxOrder(1)
        {
            my.collectPrimary(first);
        }

        Cluster:: ~Cluster() noexcept
        {

        }


        Cluster::ConstInterface & Cluster:: surrogate() const noexcept
        {
            return my;
        }

        bool Cluster:: accepts(const Equilibrium &eq) const  
        {
            assert( !my.has(eq) );
            for(const ENode *node=my.head;node;node=node->next)
            {
                const Components &mine = **node;
                if(mine.linkedTo(eq)) return true;
            }
            return false;
        }

        bool Cluster:: accepts(const Cluster &cl) const
        {
            for(const ENode *node=cl->head;node;node=node->next)
            {
                if(accepts(**node)) return true;
            }
            return false;
        }

        void Cluster:: addPrimary(const Equilibrium &eq)
        {
            assert(accepts(eq));
            my.collectPrimary(eq);
        }


        void Cluster:: addPrimary(Cluster * const cl)
        {
            assert(0!=cl);
            AutoPtr<const Cluster> ptr(cl);
            my.collectPrimary(cl->my);
        }

        
        

        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            assert(cl->size>0);
            os << '\t' << '{' << std::endl;
            os << '\t' << '\t' << '#' << cl->species << std::endl;
            for(const ENode *en=cl->head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const Fragment    &fr = *cl;
                fr.print(os << '\t' << '\t' << '@', eq);
                const xReal  K = eq.K(0);
                const real_t l = K.log10();
                os << "'10^(" << l << ")'";
                os << std::endl;
            }
            os << '\t' << '\t' << "Nu=" << cl->iTopology << std::endl;
            for(size_t i=1;i<=cl.order.size();++i)
            {
                const EList &el = cl.order[i];
                if(el.size>0)
                {
                    os << "\t\torder #" << std::setw(3) << i << " : " << el.size << std::endl;
                }
            }
            os << "\t\ttotal      : " << cl->size << std::endl;
            if(cl.conserved.list.size>0) os << "\t\tconserved = " << cl.conserved.list << std::endl;
            if(cl.unbounded.list.size>0) os << "\t\tunbounded = " << cl.unbounded.list << std::endl;
            if(cl.reacOnly.list.size>0)  os << "\t\treacOnly  = " << cl.reacOnly.list << std::endl;
            if(cl.prodOnly.list.size>0)  os << "\t\tprodOnly  = " << cl.prodOnly.list << std::endl;
            if(cl.rigorous.list.size>0)  os << "\t\trigorous  = " << cl.rigorous.list << std::endl;
            if(cl.tolerant.list.size>0)  os << "\t\ttolerant  = " << cl.tolerant.list << std::endl;

            os << '\t' << '}';
            return os;
        }


      


    }

}

