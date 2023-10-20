
//! \file

#ifndef Y_Chemical_Reactive_Group_Included
#define Y_Chemical_Reactive_Group_Included 1

#include "y/chem/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class EqGroup : public Object, public EqRepo
        {
        public:
            typedef CxxListOf<EqGroup> List;
            explicit EqGroup(const Equilibrium &first);
            virtual ~EqGroup() noexcept;
            EqGroup(const EqGroup &);

            EqGroup *next;
            EqGroup *prev;

        private:
            Y_DISABLE_ASSIGN(EqGroup);
        };

        class EqGroups : public Object, public EqGroup::List
        {
        public:
            explicit EqGroups() noexcept;
            virtual ~EqGroups() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqGroups);
        };

    }

}

#endif

