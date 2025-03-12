
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/fragment.hpp"
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/type/meta-list.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class ClusterType : public Fragment
        {
        public:
            explicit ClusterType() noexcept;
            virtual ~ClusterType() noexcept;

            void link(Equilibrium &eq);

            SubEList equilibria;
            SubSList species;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterType);
        };

        class Cluster : public Object, public Proxy<const ClusterType>, public Latchable
        {
        public:
            static const char * const CallSign;
            typedef CxxListOf<Cluster> List;

            explicit Cluster(Equilibrium &first);
            virtual ~Cluster() noexcept;

            void attach(Equilibrium &);
            bool accepts(const Equilibrium &) const noexcept; //!< shared species
            bool accepts(const Cluster &)     const noexcept; //!< shared species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            Y_PROXY_DECL();
            ClusterType my;
            
        public:
            Cluster *next;
            Cluster *prev;
        };


    }

}

#endif

