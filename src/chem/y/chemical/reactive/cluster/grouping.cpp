
#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Grouping:: Grouping() noexcept :
        EList(),
        species(),
        iTopology()
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
            iTopology.release();
            species.release();
            forget();
        }

        void Grouping:: upgrade()
        {
            try
            {
                if(size<=0) throw Specific::Exception(CallSign, "unexpected empty cluster!!");

                clear();
                DBOps::RevampSub(*this);
                {
                    AddressBook book;
                    for(const ENode *node=head;node;node=node->next)
                    {
                        const Equilibrium &eq = **node;
                        enroll(eq);
                        eq.addSpeciesTo(book);
                    }
                    book.sendTo(species);
                }
                DBOps::RevampSub(species);
                if(species.size<=0) throw Specific::Exception(CallSign, "no species in equilibria!!");

                // fill topology
                const size_t N = size;
                const size_t M = species.size;
                iTopology.make(N,M);
                for(const ENode *node=head;node;node=node->next)
                {
                    const Equilibrium &eq = **node;
                    eq.topology( iTopology[ eq.indx[SubLevel] ], SubLevel);
                }

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
