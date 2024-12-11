#include "y/chemical/plexus/connected.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Connected:: Connected(const Connected &_ ) :
        EList(_),
        Fragment(_),
        species(_.species),
        topology(_.topology)
        {
        }

        void Connected:: swapAll(Connected &_) noexcept
        {
            swapWith(_);
            trades(_);
            species.swapWith(_.species);
            topology.xch(_.topology);
        }


        bool Connected:: accepts(const Equilibrium &eq) const noexcept
        {
            for(ENode *en=head;en;en=en->next)
            {
                if( (**en).linkedTo(eq) ) return true;
            }
            return false;
        }
        
        void Connected:: connect(const Equilibrium &eq)
        {
            assert( !has(eq) );

            Connected backup(*this);

            try {
                *this << eq;
                for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Species &sp = (*it).actor.sp;
                    if(!species.has(sp)) species << sp;
                }

                const size_t N = DBOps::RevampSub(*this).size;
                const size_t M = DBOps::RevampSub(species).size;
                topology.make(N,M);
                fillTopology();

                if( N != MKL::Rank::Of(topology) )
                    throw Specific::Exception(CallSign, "dependent '%s'", eq.name.c_str());


            }
            catch(...)
            {
                swapAll(backup);
                throw;
            }

        }

        void Connected:: fillTopology() noexcept
        {
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.topology( topology[ eq.indx[SubLevel] ], SubLevel );
            }
        }


    }

}
