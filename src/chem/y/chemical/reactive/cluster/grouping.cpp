
#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Grouping:: Grouping() noexcept
        {

        }


        Grouping:: ~Grouping() noexcept
        {

        }

        const char * const Grouping:: CallSign = "Chemical::Cluster";

        void Grouping:: collect(const Equilibrium &eq)
        {
            for(const ENode *node=head;node;node=node->next)
            {
                const Equilibrium &mine = **node;
                if( &eq == &mine ) throw Specific::Exception(CallSign,"multiple '%s' in cluster", eq.name.c_str());
            }
            
            (*this) << eq;
            enroll(eq);
        }


        void Grouping:: update() noexcept
        {
            for(const ENode *node=head;node;node=node->next)
            {
                enroll(**node);
            }
        }

    }

}
