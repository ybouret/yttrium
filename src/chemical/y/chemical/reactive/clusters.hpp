
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Clusters : public CxxListOf<Cluster>
        {
        public:
            explicit Clusters(Equilibria &eqs, const Constants &topK, XMLog &xml) ;
            virtual ~Clusters() noexcept;

            const Readable<XReal> & K(const Real t);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Constants sharedK;
            void build(Equilibria &, XMLog &); //!< build and compile
            void merge() noexcept;             //!< merge cluster sharing species
        };
    }

}

#endif
