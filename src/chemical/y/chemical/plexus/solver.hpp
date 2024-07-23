
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/squad.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/chemical/reactive/equilibria/linearly-independent.hpp"
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

            explicit Solver(const Clusters &cls); //!< prepare resource
            virtual ~Solver() noexcept;           //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! process one cluster
            void process(XWritable       &C,
                         const Cluster   &cl,
                         const XReadable &K,
                         XMLog           &xml);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Matrix<xreal_t>        Ceq; //!< all solving phase space
            Matrix<xreal_t>        dCe; //!< Ceq-C0
            Matrix<xreal_t>        Phi; //!< derivatives
            Aftermath              afm; //!< single eq solving
            PBank                  bnk; //!< shared cache of PNode
            Prospect::Series       pps; //!< current prospect
            LinearlyIndependentSet lis; //!< find out basis
            Squad::List            sql; //!< squad list
            Squad::Pool            sqp; //!< squad bpool

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            void    sqFree() noexcept;
            Squad * sqMake(Prospect &first);
            void    sqDrop(Squad *) noexcept;
            void    sqFrom(const PList        &plist,
                           const Matrix<bool> &attached,
                           XMLog              &xml);

        };

    }

}

#endif
