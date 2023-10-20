
#include "y/chem/reactive/group.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        EqGroup:: ~EqGroup() noexcept
        {
        }

        EqGroup:: EqGroup(const Equilibrium &first)  :
        Object(),
        EqRepo(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        EqGroup:: EqGroup(const EqGroup &other) :
        Object(),
        EqRepo(other),
        next(0),
        prev(0)
        {
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        EqGroups:: EqGroups() noexcept : Object(), EqGroup::List()
        {
        }

        EqGroups:: ~EqGroups() noexcept
        {
        }

    }
}
