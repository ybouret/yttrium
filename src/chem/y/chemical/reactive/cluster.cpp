
#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: Cluster(const Equilibrium &first) :
        Proxy<const Grouping>(),
        eqs(),
        next(0),
        prev(0)
        {
            eqs.collect(first);
        }

        Cluster:: ~Cluster() noexcept
        {

        }


        Cluster::ConstInterface & Cluster:: surrogate() const noexcept
        {
            return eqs;
        }

        bool Cluster:: accepts(const Equilibrium &eq) const  
        {
            assert( !eqs.has(eq) );
            for(const ENode *node=eqs.head;node;node=node->next)
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

        void Cluster:: add(const Equilibrium &eq)
        {
            assert(accepts(eq));
            eqs.collect(eq);
        }

        void Cluster:: add(Cluster * const cl) noexcept
        {
            assert(0!=cl);
            AutoPtr<const Cluster> ptr(cl);
            eqs.collect(cl->eqs);
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
            os << '\t' << '}';
            return os;
        }





    }

}

