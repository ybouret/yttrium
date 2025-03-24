//! \file


#ifndef Y_Chemical_Reactor_Included
#define Y_Chemical_Reactor_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/apex/api/ortho/family.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/algebra/lu.hpp"

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

        typedef CxxArray<XAdd,MemoryModel>    Summator; //!< alias
        typedef Vector<String,MemoryModel>    Strings;  //!< alias

        typedef CxxArray<XArray,MemoryModel>  XArrays;   //!< alias
        typedef CxxArray<XMatrix,MemoryModel> XMatrices; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Reactor for one Cluster
        //
        //
        //______________________________________________________________________
        class Reactor : public Quantized
        {
        public:
            static const char * const ProfileExt;   //!< "pro"
            static bool               EmitProfiles; //!< default to false, emit profiles
            static bool               MonitorScore; //!< default to false, monitor score
            static const char * const GetRunning;   //!< "GetRunning"
            static const char * const NarrowDown;   //!< "NarrowDown"
            static const char * const QueryRates;   //!< "QueryRates"
            static const char * const GenerateNR;   //!< "GenerateNR"

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


            const Cluster & cluster;   //!< persistent cluster
            Aftermath       solve1D;   //!< computing 1D solution
            XAdd            x_score;   //!< helper to compute score
            OutList         running;   //!< running equilibria
            ESolo           basis;     //!< basis of running equilibria
            XMatrix         Ceq;       //!< workspace to store 1D solution
            XArray          Cini;      //!< initial state
            XArray          Cend;      //!< final   state
            XArray          Ctry;      //!< trial   state
            XArray          dC;        //!< trial    deltaC
            XArray          Cwin;      //!< store wining state
            const QMetrics  qMetrics;  //!< |species|
            QVCache         qVCache;   //!< for vectors
            QFamily         qFamily;   //!< for building basis
            Summator         rate;      //!< for computing rates
            MKL::LU<xreal_t> lu;
            XArrays          xiArr;     //!< xiArr[[1:1],...,[1:n]]

            Strings         profiles;  //!< save guess names
            Reactor *       next;
            Reactor *       prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            static bool     IsRunning(const Outcome &) noexcept;
            static SignType ByDecreasingAX(const OutNode * const lhs, const OutNode * const rhs) noexcept;
            static SignType ByIncreasingSC(const OutNode * const lhs, const OutNode * const rhs) noexcept;


            //! RMS of running affinities
            xreal_t         score(const XReadable &, const Level);
            
            //! evaluate running after removing crucial
            xreal_t         getRunning(XMLog &xml, XWritable &C0, const XReadable &K0);


            //! narrow down running extents to optized score
            xreal_t         narrowDown(XMLog &xml, const xreal_t S0);

            //! using optimized xi as virtual rates
            xreal_t         queryRates(XMLog &xml, const xreal_t S0);


            //! using Newton-Raphson algorithm
            xreal_t         generateNR(XMLog &xml, const xreal_t S0, const XReadable & K0);

            //! optimize with Sini @Cini and prepared Cend
            xreal_t         optimize1D(const xreal_t Sini);

            //! convert rates into dC and build optimized Cend = Cini + rho * dC, clipping rho
            xreal_t         optimizedC(XMLog &            xml,
                                       const xreal_t      S0,
                                       xreal_t            rho,
                                       const char * const uid);

            //! rates + x * Nu_eq
            void            increaseRates(const xreal_t x, const Components &eq);
            void            finalize(XMLog &xml, XWritable &C0, const bool exact);

            //! check Stry with precomputed Ctry from a Method
            bool            converged(XMLog &            xml,
                                      const xreal_t      Stry,
                                      const char * const Mtry,
                                      xreal_t      &     Swin,
                                      const char * &     Mwin);

            void            saveCurrentProfile(const String &, const size_t);             //!< save Cini->Cend, change Ctry!!
            void            eraseOlderProfiles() noexcept;                                //!< remove .pro
            static void     EmitGnuPlotTracing(std::ostream &os, const String &baseName); //!< helper
            void            emitGnuPlotTracing(std::ostream &os) const;                   //!< emit command line
        };


    }

}

#endif

