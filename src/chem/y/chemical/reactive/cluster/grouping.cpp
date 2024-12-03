
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


        void Grouping:: collect(Grouping &other)
        {
            assert(this != &other);
            mergeTail(other);
            upgrade();
        }


        void Grouping:: clear() noexcept
        {
            Coerce(species).release();
            forget();
        }

        void Grouping:: upgrade()
        {
            try
            {
                clear();
                DBOps::RevampSub(*this);
                SList &mine = Coerce(species);
                {
                    AddressBook book;
                    for(const ENode *node=head;node;node=node->next)
                    {
                        const Equilibrium &eq = **node;
                        enroll(eq);
                        eq.addSpeciesTo(book);
                    }
                    book.sendTo(mine);
                }
                DBOps::RevampSub(mine);
            }
            catch(...)
            {
                release();
                clear();
                throw;
            }


        }

    }

}
