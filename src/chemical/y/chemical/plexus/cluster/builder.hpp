//! \file

#ifndef Y_Chemical_ClusterBuilder_Included
#define Y_Chemical_ClusterBuilder_Included 1

#include "y/chemical/plexus/cluster/content.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class ClusterNode : public Quantized, public ClusterContent::MutablePointer
        {
        public:
            explicit ClusterNode(const ClusterContent::MutablePointer  &ptr) noexcept :
            ClusterContent::MutablePointer(ptr),
            next(0),
            prev(0)
            {
            }

            virtual ~ClusterNode() noexcept {}
            ClusterNode *next;
            ClusterNode *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterNode);
        };

        class ClusterBuilder : public Proxy<const ListOf<ClusterNode> >
        {
        public:
            explicit ClusterBuilder(XMLog &xml, Equilibria &eqs);
            virtual ~ClusterBuilder() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterBuilder);
            Y_PROXY_DECL();
            void startClusterFor(Equilibrium &eq);
            
            CxxListOf<ClusterNode> cls;
        };
    }

}

#endif

