
#include "y/jive/group.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Group::~Group() noexcept {}

        Group:: Group() noexcept : maxNameSize(0)
        {
        }

        void Group:: updateWith(const Entity &ent) noexcept
        {
            Coerce(maxNameSize) = Max(maxNameSize,ent.name->size());
        }
    }

}
