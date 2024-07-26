
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/qbuilder.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Solver
        //
        //
        //______________________________________________________________________
        class Solver
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup resources to solve clusters
            explicit Solver(const Clusters &cls) :
            afm(),
            ceq(cls.maxEPC,cls.maxSPC),
            phi(cls.maxSPC),
            obj(cls.maxEPC),
            pps(cls.maxEPC),
            Cin(cls.maxSPC,0),
            Cex(cls.maxSPC,0),
            Cws(cls.maxSPC,0),
            bnk(),
            qdb(),
            ppb(0),
            pcl(0)
            {
                for(const Cluster *cl=cls->head;cl;cl=cl->next)
                {
                    qdb(cl->Nu.rows,cl->species.size,bnk);
                }
            }

            //! cleanup
            virtual ~Solver() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build objective function from C at level L
            xreal_t objectiveFunction(const XReadable &C,
                                      const Level      L);

            xreal_t operator()(const xreal_t u);

            //! run
            void run(const Cluster   &cl,
                     XWritable       &Ctop,
                     const XReadable &Ktop,
                     XMLog           &xml);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Aftermath           afm; //!< aftermath computation
            XMatrix             ceq; //!< storing results
            XArray              phi; //!< temporary gradients
            CxxSeries<xreal_t>  obj; //!< temporary objectice function values
            CxxSeries<Prospect> pps; //!< prospect series
            XArray              Cin;
            XArray              Cex;
            XArray              Cws;
            PBank               bnk; //!< shared bank of PNODE
            QBuilders           qdb; //!< QBuilder for different clusters
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            QBuilder           *ppb; //!< pointer to prospect base
            Cluster            *pcl; //!< pointer to current cluster
        };

    }
}

#endif

