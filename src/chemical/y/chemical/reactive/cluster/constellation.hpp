
//! \file

#ifndef Y_Chemical_Cluster_Constellation_Included
#define Y_Chemical_Cluster_Constellation_Included 1

#include "y/chemical/reactive/cluster/combinatorics.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Controller : public Object
        {
        public:
            typedef Components::ConstIterator ConstIterator;

            explicit Controller(const Equilibrium &eq,
                                const AddressBook &conserved) :
            primary(eq),
            components(),
            next(0),
            prev(0)
            {
                Components &self = Coerce(components);
                for(ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Component &component = *it;
                    if( conserved.search(component.sp) ) self(component);
                }
            }

            virtual ~Controller() noexcept {}

            bool isEquivalentTo(const Controller &other) const noexcept
            {
                return components.isAnalogousTo(other.components);
            }

            const Equilibrium &primary;
            const Components   components;
            Controller        *next;
            Controller        *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Controller);
        };

        typedef CxxListOf<Controller> Controllers;

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

            const EList       hasOnlyReac;
            const EList       hasOnlyProd;
            const Controllers controllers;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConstellation);
        };

    }

}

#endif
