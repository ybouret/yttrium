
#include "y/chemical/entity.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        const String & Entity:: key() const noexcept
        {
            return name;
        }

        Entity:: ~Entity() noexcept
        {
        }


        void Entity:: setIndices(const size_t level)
        {
            if(level<=0) throw Specific::Exception("Chemical::Entity", "'%s' with zero index", name.c_str());
            for(unsigned i=0;i<Levels;++i) Coerce(indx[i]) = level;
        }

        Entity:: Entity(const CopyOf_ &, const Entity &other) :
        name(other.name),
        indx()
        {
            memcpy( &Coerce(indx[0]), other.indx, sizeof(indx));
        }

    }

}

