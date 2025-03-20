
//! \file


#ifndef Y_Chemical_Reactor_Included
#define Y_Chemical_Reactor_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/apex/api/ortho/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        // Declarations
        //
        //
        //______________________________________________________________________
        typedef Small::SoloHeavyList<Outcome> OutList;  //!< alias
        typedef OutList::NodeType             OutNode;  //!< alias
        typedef Apex::Ortho::Vector           QVector;  //!< alias
        typedef Apex::Ortho::Family           QFamily;  //!< alias
        typedef Apex::Ortho::VCache           QVCache;  //!< alias
        typedef Apex::Ortho::Metrics          QMetrics; //!< alias

        typedef CxxArray<XAdd,MemoryModel>    Summator;

        //______________________________________________________________________
        //
        //
        //
        //! Reactor for one Cluster
        //
        //
        //______________________________________________________________________
        class Reactor
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Reactor(const Cluster &); //!< setup
            virtual ~Reactor() noexcept;       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! solve topLevel
            void operator()(XMLog &xml, XWritable &C0, const XReadable &K0);



            //! score(Ctry=Cini*(1-u)+Cend*u);
            xreal_t operator()(const xreal_t u);


            const Cluster & cluster;  //!< persistent cluster
            Aftermath       solve1D;  //!< computing 1D solution
            XAdd            x_score;  //!< helper to compute score
            OutList         running;  //!< running equilibria
            ESolo           basis;    //!< basis of running equilibria
            XMatrix         Ceq;      //!< workspace to store 1D solution
            XArray          Cini;     //!< initial state
            XArray          Cend;     //!< final   state
            XArray          Ctry;     //!< trial   state
            XArray          dC;       //!< trial    deltaC
            const QMetrics  qMetrics; //!< |species|
            QVCache         qVCache;  //!< for vectors
            QFamily         qFamily;  //!< for building basis
            Summator        rate;     //!< for computing rates
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            static bool     IsRunning(const Outcome &) noexcept;
            static SignType ByDecreasingAX(const OutNode * const lhs, const OutNode * const rhs) noexcept;
            static SignType ByIncreasingSC(const OutNode * const lhs, const OutNode * const rhs) noexcept;

            xreal_t         initialize(XMLog &xml, XWritable &C0, const XReadable &K0);
            xreal_t         ameliorate(XMLog &xml, const xreal_t S0);
            xreal_t         queryRates(XMLog &xml, const xreal_t S0);
            real_t          optimize1D(const xreal_t Sini);        //!< with Sini @Cini and loaded Cend
            xreal_t         score(const XReadable &, const Level); //!< RMS(affinities)


        };


    }

}

#endif

