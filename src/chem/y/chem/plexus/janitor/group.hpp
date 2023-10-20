
//! \file

#ifndef Y_Chemical_Janitor_Group_Included
#define Y_Chemical_Janitor_Group_Included 1

#include "y/chem/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class JGroup : public Object, public EqRepo
        {
        public:
            typedef CxxListOf<JGroup> List;
            explicit JGroup() noexcept;
            virtual ~JGroup() noexcept;
            JGroup(const JGroup &);

            JGroup *next;
            JGroup *prev;

        private:
            Y_DISABLE_ASSIGN(JGroup);
        };

        class JGroups : public Object, public JGroup::List
        {
        public:
            explicit JGroups() noexcept;
            virtual ~JGroups() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(JGroups);
        };

    }

}

#endif

