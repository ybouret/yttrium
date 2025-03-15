//! \file


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

        class ClusterBuilder
        {
        public:
            typedef ClusterContent::MutablePointer MPtr;

            class ClusterNode : public Object, public MPtr
            {
            public:
                explicit ClusterNode(const MPtr &ptr) noexcept :
                Object(),
                MPtr(ptr),
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
            
            explicit ClusterBuilder(XMLog &xml, Equilibria &eqs);
            virtual ~ClusterBuilder() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterBuilder);
            CxxListOf<ClusterNode> cls;
            void startClusterFor(Equilibrium &eq);
        };
    }

}

#endif

