
//! \file

#ifndef Y_Chemical_Cluster_Constellation_Included
#define Y_Chemical_Cluster_Constellation_Included 1

#include "y/chemical/reactive/cluster/combinatorics.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Regulator : public Object, public Proxy<const Components>
        {
        public:
            typedef Components::ConstIterator ConstIterator;

            explicit Regulator(const Equilibrium &eq,
                               const AddressBook &conserved) :
            Proxy<const Components>(),
            primary(eq),
            next(0),
            prev(0)
            {
                for(ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Component &component = *it;
                    if( conserved.search(component.sp)) components(component);
                }
            }

            virtual ~Regulator() noexcept {}

            const Equilibrium &primary;
            Regulator         *next;
            Regulator         *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Regulator);
            Components components;
            virtual ConstInterface & surrogate() const noexcept { return components; }
        };

        //______________________________________________________________________
        //
        //
        //
        //! Compute all possible equilibria from topology
        //
        //
        //______________________________________________________________________
        class ClusterConstellation : public ClusterCombinatorics
        {
        public:
            explicit ClusterConstellation(Equilibria        &eqs,
                                          const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);
            virtual ~ClusterConstellation() noexcept;

            const AddressBook hasOnlyReac;
            const AddressBook hasOnlyProd;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConstellation);
        };

    }

}

#endif
