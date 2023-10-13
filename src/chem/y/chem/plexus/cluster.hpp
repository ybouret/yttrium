//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/stream/xmlog.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::BareLightList<const Species>     SpRepo;
        typedef SpRepo::NodeType                        SpNode;

        typedef Small::BareLightList<const Equilibrium> EqRepo;
        typedef EqRepo::NodeType                        EqNode;

        typedef CxxArray<Equilibrium * const> EqArray;
        typedef CxxArray<Species     * const> SpArray;

        class Cluster : public Object
        {
        public:
            explicit Cluster(const Equilibrium &first);
            virtual ~Cluster() noexcept;
            
            bool tiedTo(const Equilibrium &) const noexcept;
            bool tiedTo(const Cluster &)     const noexcept;
            void enroll(const Equilibrium &);
            void compile(Equilibria &all, XMLog &);


            const EqRepo                 eqs;
            const SpRepo                 lib;
            const AutoPtr<const EqArray> edb;
            const AutoPtr<const SpArray> sdb;

            const Matrix<int>      Nu;    //!< topology
            const Matrix<unsigned> Qm;    //!< conservation matrix
            Cluster *         next;
            Cluster *         prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void buildConservations(XMLog &);
        };

        typedef CxxListOf<Cluster> Clusters;
    }
}

#endif
