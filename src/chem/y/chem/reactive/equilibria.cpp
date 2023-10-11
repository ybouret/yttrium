#include "y/chem/reactive/equilibria.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibria:: Equilibria() : db() {}

        Equilibria:: ~Equilibria() noexcept
        {

        }

        Equilibria::ConstInterface & Equilibria:: surrogate() const noexcept
        {
            return db;
        }

        const char * const Equilibria::CallSign = "Chemical::Equilibria";

        void Equilibria:: submit(Equilibrium *eq)
        {
            assert(0!=eq);

            // try to insert it in database
            {
                const Equilibrium::Pointer ep(eq);
                if(!db.insert(ep))
                    throw Specific::Exception(CallSign,"multiple <%s>", eq->name.c_str());
            }

            // initial indices
            const size_t idx = db.size();
            for(unsigned i=0;i<Levels;++i) Coerce(eq->indx[i]) = idx;

            // for format
            updateWith(*eq);
        }

    }

}

