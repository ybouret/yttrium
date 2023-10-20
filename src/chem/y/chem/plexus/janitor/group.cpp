
#include "y/chem/plexus/janitor/group.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        JGroup:: ~JGroup() noexcept
        {
        }

        JGroup:: JGroup() noexcept :
        Object(),
        EqRepo(),
        next(0),
        prev(0)
        {

        }

        JGroup:: JGroup(const JGroup &other) :
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
        JGroups:: JGroups() noexcept : Object(), JGroup::List()
        {
        }

        JGroups:: ~JGroups() noexcept
        {
        }

    }
}
