
//! \file


#ifndef Y_Chemical_Reactor_Included
#define Y_Chemical_Reactor_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/aftermath.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::SoloHeavyList<Outcome> OutList;
        typedef OutList::NodeType             OutNode;

        class Reactor
        {
        public:

            explicit Reactor(const Cluster &persistentCluster) :
            cluster(persistentCluster),
            running(),
            Ceq(cluster->equilibria->size,cluster->species->size),
            Cini(cluster->species->size),
            Cend(cluster->species->size),
            Ctry(cluster->species->size)
            {
                running.proxy->reserve(Ceq.rows);
            }

            virtual ~Reactor() noexcept
            {
            }

            //! solve topLevel
            void operator()(XMLog &xml, XWritable &C0, const XReadable &K0);

            static SignType ByDecreasingAX(const OutNode * const lhs, const OutNode * const rhs) noexcept;
            static SignType ByIncreasingSC(const OutNode * const lhs, const OutNode * const rhs) noexcept;

            xreal_t score(const XReadable &C,
                          const Level      L);

            //! score(Ctry=Cini*(1-u)+Cend*u);
            xreal_t operator()(const xreal_t u);


            const Cluster & cluster;  //!< persistent cluster
            Aftermath       solve1D;  //!< computing 1D solution
            XAdd            x_score;  //!< helper to compute score
            OutList         running;  //!< running equilibria
            XMatrix         Ceq;      //!< workspace to store 1D solution
            XArray          Cini;     //!< initial state
            XArray          Cend;     //!< final   state
            XArray          Ctry;     //!< trial   state

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            static bool IsRunning(const Outcome &out) noexcept
            {
                assert(Blocked!=out.st);
                return Running == out.st;
            }

            void   initialize(XMLog &xml, XWritable &C0, const XReadable &K0);
            void   ameliorate(XMLog &xml);
            real_t optimize1D(const xreal_t Sini); //!< from Sini @Cini


        };


    }

}

#endif

