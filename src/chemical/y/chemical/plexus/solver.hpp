
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/chemical/reactive/equilibria/linearly-independent.hpp"
#include "y/container/cxx/series.hpp"


namespace Yttrium
{
    namespace Chemical
    {


        
        class Solver
        {
        public:
            explicit Solver(const Clusters &cls);
            virtual ~Solver() noexcept;


            void process(XWritable       &C,
                         const Cluster   &cl,
                         const XReadable &K,
                         XMLog           &xml);


            Matrix<xreal_t>        Ceq;
            Matrix<xreal_t>        Phi;
            Aftermath              afm;
            Prospect::Series       pps;
            LinearlyIndependentSet lis;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };

    }

}

#endif
