//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/cluster/conservation/laws.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        class Cluster : public Proxy<const EList>, public Party
        {
        public:
            typedef CxxListOf<Cluster> List;
            typedef Conservation::Laws CLaws;
            typedef Conservation::Law  CLaw;

            virtual ~Cluster() noexcept;
            explicit Cluster(const Equilibrium &first);

            bool accept(const Equilibrium &) const noexcept;
            bool accept(const Cluster     &) const noexcept;
            void compile(XMLog &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            virtual ConstInterface & surrogate() const noexcept;
            EList       eqs;

        public:
            const SList            species; //!< species in this cluster
            const Matrix<int>      Nu;      //!< primary topology
            const Matrix<unsigned> Qm;      //!< conservation matrix
            const AutoPtr<CLaws>   laws;    //!< matching laws

            Cluster *   next;
            Cluster *   prev;

        private:
            void buildConservations(XMLog &);
        };
    }

}


#endif

