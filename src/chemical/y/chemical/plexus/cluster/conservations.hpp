
//! \file

#ifndef Y_Chemical_ClusterConservations_Included
#define Y_Chemical_ClusterConservations_Included 1

#include "y/chemical/plexus/cluster/topology.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Matrix<unsigned> uMatrix;

        namespace Conservation
        {
            class Law : public Actors
            {
            public:
                typedef CxxListOf<Law> List;

                explicit Law() noexcept;
                virtual ~Law() noexcept;

                Law *next;
                Law *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
            };



            class Laws : public Proxy<const Law::List>, public Assembly
            {
            public:
                explicit Laws() noexcept;
                virtual ~Laws() noexcept;
                Y_OSTREAM_PROTO(Laws);

                //! append and latch
                void add(Law *const law) noexcept;

            private:
                Y_DISABLE_ASSIGN(Laws);
                Y_PROXY_DECL();
                Law::List      list;
            public:
                const AuxSList species;
            };
        }




        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(XMLog &, const ClusterContent::Pointer &);
            virtual ~ClusterConservations() noexcept;

            const uMatrix            preserved; //!< preserved matrix
            const Conservation::Laws ordinance; //!< matching laws
            const SList              conserved; //!< conserved species
            const SList              unbounded; //!< unbounded species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };

    }

}


#endif

