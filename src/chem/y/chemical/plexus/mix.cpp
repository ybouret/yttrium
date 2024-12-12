
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Mix::ConstInterface & Mix:: surrogate() const noexcept { return my; }

        Mix:: ~Mix() noexcept
        {
        }


        const char * const Mix:: CallSign = "Chemical::Mix";
        
        Mix:: Mix(const Equilibrium &first) :
        Proxy<const Connected>(),
        my(first),
        next(0),
        prev(0)
        {
        }

        bool Mix:: accepts(const Equilibrium &eq) const noexcept
        {
            return my.accepts(eq);
        }

        bool Mix:: accepts(const Mix &mix) const noexcept
        {
            for(const ENode *en=mix->head;en;en=en->next)
            {
                if( accepts( **en ) ) return true;
            }
            return false;
        }

        void Mix:: addPrimary(const Equilibrium &eq)
        {
            my.connect(eq);
        }


        void Mix:: addPrimary(const Mix &mix)
        {
            for(const ENode *en=mix->head;en;en=en->next)
            {
                my.connect(**en);
            }
        }


        std::ostream & operator<<(std::ostream &os, const Mix &mix)
        {
            assert(mix->size>0);
            os << "\t{" << std::endl;
            os << "\t\tspecies=" << mix->species << std::endl;
            for(const ENode *en=mix->head;en;en=en->next)
            {
                mix->print(os << "\t\t@",**en) << std::endl;
            }
            os << "\t\ttopology=" << mix->topology << std::endl;
            os << "\t}";
            return os;
        }

    }

}
