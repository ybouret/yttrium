
#include "y/lingo/entity.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Entity:: ~Entity() noexcept
        {
        }

        const String & Entity:: key() const noexcept
        {
            return *name;
        }

        Entity::Entity(const Entity &other) noexcept :
        Counted(),
        name(other.name)
        {

        }

        std::ostream & operator<<(std::ostream &os, const Entity &ent)
        {
            static const char quote = '\'';
            os << quote << ent.name << quote;
            return os;
        }
    }

}
