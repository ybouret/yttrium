
#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/rank.hpp"

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

        Grouping:: Grouping(const Grouping &_) :
        EList(_),
        Fragment(_),
        species(_.species),
        iTopology(_.iTopology)
        {

        }

        void Grouping:: xch(Grouping &_) noexcept
        {
            swapWith(_);                    // EList
            trades(_);                      // Fragment
            species.swapWith(_.species);    // species
            iTopology.xch(_.iTopology);     // topology
        }


        Grouping:: ~Grouping() noexcept
        {

        }

        const char * const Grouping:: CallSign = "Chemical::Cluster";

        void Grouping:: chk(const Equilibrium &eq) const
        {
            for(const ENode *node=head;node;node=node->next)
            {
                const Equilibrium &mine = **node;
                if( &eq == &mine ) throw Specific::Exception(CallSign,"multiple '%s' in cluster", eq.name.c_str());
            }
        }

        void Grouping:: collectPrimary(const Equilibrium &eq)
        {
            // check
            chk(eq);

            // backup
            Grouping G(*this);


            try {
                //--------------------------------------------------------------
                //
                // expand list of equilibria
                //
                //--------------------------------------------------------------
                (*this) << eq;
                assert(size>0);

                //--------------------------------------------------------------
                //
                // expand list of species
                //
                //--------------------------------------------------------------
                for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Species &sp = (*it).actor.sp;
                    if(!species.has(sp)) species << sp;
                }

                if(species.size<=0)
                    throw Specific::Exception(CallSign,"no species in equilibria");

                //--------------------------------------------------------------
                //
                // revamp
                //
                //--------------------------------------------------------------
                DBOps::RevampSub(*this);
                DBOps::RevampSub(species);


                //--------------------------------------------------------------
                //
                // fill topology
                //
                //--------------------------------------------------------------
                const size_t N = size;
                const size_t M = species.size;
                iTopology.make(N,M);
                for(const ENode *node=head;node;node=node->next)
                {
                    const Equilibrium &eq = **node;
                    eq.topology( iTopology[ eq.indx[SubLevel] ], SubLevel);
                }

                //--------------------------------------------------------------
                //
                // Check topology
                //
                //--------------------------------------------------------------
                if( MKL::Rank::Of(iTopology) != size )
                    throw Specific::Exception(CallSign, "primary equilibria are not independent!!");

                // keep format up to date
                enroll(eq);
            }
            catch(...)
            {
                xch(G); // restore state
                throw;
            }

        }


        void Grouping:: collectPrimary(Grouping &other)
        {
            assert(this != &other);
            Grouping current(*this);
            try {
                for(const ENode *en=other.head;en;en=en->next)
                    collectPrimary(**en);
            }
            catch(...)
            {
                xch(current);
                throw;
            }
        }


        void Grouping:: collectReplica(const Equilibrium &eq)
        {
            chk(eq);

            Grouping G(*this);

            // shared all species are included
            for(Components::ConstIterator it=eq->begin();it!=eq->end();++it)
            {
                const Component &cm = *it;
                const Species   &sp = cm.actor.sp;
                if( !species.has(sp) )
                    throw Specific::Exception(CallSign,"missing '%s' in replica '%s'", sp.name.c_str(), eq.name.c_str() );
            }

            // register
            try
            {
                (*this) << eq;
                DBOps::RevampSub(*this);
                enroll(eq);
            }
            catch(...)
            {
                xch(G);
                throw;
            }



        }

        

    }

}
