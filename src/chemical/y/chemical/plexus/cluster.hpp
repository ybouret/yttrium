
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/plexus/cluster/type.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Clusters;

        typedef Matrix<int>      iMatrix;
        typedef Matrix<unsigned> uMatrix;

        class Cluster : public Object, public Proxy<const ClusterType>, public Latchable
        {
        public:
            static const char * const CallSign;
            typedef CxxListOf<Cluster> List;

            explicit Cluster(Equilibrium &first);
            virtual ~Cluster() noexcept;
            Y_OSTREAM_PROTO(Cluster);


            // Construction Methods
            void attach(Equilibrium &);                       //!< sanity check and link equilibrium/species
            bool accepts(const Equilibrium &) const noexcept; //!< shared species
            bool accepts(const Cluster &)     const noexcept; //!< shared species
            void attach(Cluster &);                           //!< steal content (if not latched)


            const iMatrix topology;
            const iMatrix topologyT;
            const uMatrix preserved;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            friend class Clusters;
            Y_PROXY_DECL();
            void compile(XMLog &);
            void conservations(XMLog &);
            void combinatorics(XMLog &);
            ClusterType my;

        public:
            Cluster *next;
            Cluster *prev;

        };

    }

}

#endif

