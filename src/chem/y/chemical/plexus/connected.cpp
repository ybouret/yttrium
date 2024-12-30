#include "y/chemical/plexus/connected.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Connected::CallSign = "Chemical::Connected";


        Connected:: Connected(const Equilibrium &first,
                              const unsigned     cindx) :
        EList(),
        Fragment(),
        species(),
        sformat(),
        topology(),
        hallmark(cindx)
        {
            connect(first);
        }

        Connected:: Connected(const Connected &_ ) :
        EList(_),
        Fragment(_),
        species(_.species),
        sformat(_.sformat),
        topology(_.topology),
        hallmark(_.hallmark)
        {
        }

        Connected:: ~Connected() noexcept {}

        void Connected:: swapAll(Connected &_) noexcept
        {
            swapWith(_);
            trades(_);
            species.swapWith(_.species);
            sformat.tradeFor(_.sformat);
            topology.xch(_.topology);
            CoerceSwap(hallmark, _.hallmark);
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
                // add eq
                *this << eq;

                // add species
                for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Species &sp = (*it).actor.sp;
                    if(!species.has(sp))
                    {
                        species << sp;
                        sformat.enroll(sp);
                    }
                }

                // new topology
                const size_t N = DBOps::RevampSub(*this).size;
                const size_t M = DBOps::RevampSub(species).size;
                topology.make(N,M);
                fillTopology();

                // check it
                if( N != MKL::Rank::Of(topology) )
                    throw Specific::Exception(CallSign, "dependent '%s'", eq.name.c_str());

                // prepare format
                enroll(eq);

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

        void Connected:: replica(const Equilibrium &eq)
        {
            assert( !has(eq) );
            (*this) << eq;
            DBOps::RevampSub(*this);
            enroll(eq);
        }

    }

}
