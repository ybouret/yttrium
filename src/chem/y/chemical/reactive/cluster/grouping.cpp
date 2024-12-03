
#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Grouping:: Grouping() noexcept :
        EList(),
        species()
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
            upgrade();
        }


        void Grouping:: update() noexcept
        {
            forget();
            for(const ENode *node=head;node;node=node->next)
                enroll(**node);
        }

        void Grouping:: upgrade()
        {
            try
            {
                update();
                AddressBook book;
                
            }
            catch(...)
            {
                release();
                forget();
                Coerce(species).release();
                throw;
            }

        }

    }

}
