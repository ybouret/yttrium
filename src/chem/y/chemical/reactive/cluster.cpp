
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
        next(0),
        prev(0),
        indx(0)
        {
            my.collect(first);
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
            my.collect(eq);
        }

        void Cluster:: addPrimary(Cluster * const cl)
        {
            assert(0!=cl);
            AutoPtr<const Cluster> ptr(cl);
            my.collect(cl->my);
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

