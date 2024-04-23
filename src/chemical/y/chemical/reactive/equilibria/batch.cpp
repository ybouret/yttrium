#include "y/chemical/reactive/equilibria/batch.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Batch:: Batch(const Equilibrium &first) : 
        Oversized(),
        Fragment(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        Batch:: ~Batch() noexcept {}
        
        bool Batch:: sharesSpeciesWith(const Equilibrium &eq) const noexcept
        {
            for(const ENode *en=head;en;en=en->next)
            {
                if( eq.sharesSpeciesWith(**en) ) return true;
            }
            return false;
        }

        bool Batch:: sharesSpeciesWith(const Batch &batch) const noexcept
        {
            for(const ENode *en=head;en;en=en->next)
            {
                if( batch.sharesSpeciesWith(**en) ) return true;
            }
            return false;
        }

        void Batch:: finalize()
        {
            LightSort::MakeSubLevel(*this);
            prepareSpecies();
        }

        void Batch:: prepareSpecies()
        {
            AddressBook   book;
            SList        &spec = Coerce(species); spec.free();
            for(const ENode *en=head;en;en=en->next)                         (**en).recordSpeciesInto(book);
            for(AddressBook::Iterator it=book.begin(); it!=book.end(); ++it) spec << *static_cast<const Species *>( *it );
            LightSort::MakeSubLevel(spec);
        }

    }

}

