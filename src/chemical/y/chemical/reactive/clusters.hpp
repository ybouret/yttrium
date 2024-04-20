
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
            explicit Clusters(Equilibria &eqs, const Readable<XReal> &topK, XMLog &xml) ;
            virtual ~Clusters() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            void build(Equilibria &, const Readable<XReal> &topK, XMLog &); //!< build and compile
            void merge() noexcept;                                          //!< merge cluster sharing species
        };
    }

}

#endif
