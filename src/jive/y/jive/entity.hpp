
#ifndef Y_Jive_Entity_Included
#define Y_Jive_Entity_Included 1


#include "y/jive/tag.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Entity : public Object, public Counted
        {
        public:
            template <typename LABEL> inline
            explicit Entity(const LABEL &id) : 
            Object(),
            Counted(),
            name(id)
            {}

            Entity(const Entity &) noexcept;
            virtual ~Entity()      noexcept;

            const String & key() const noexcept;

            const Tag name;

        private:
            Y_DISABLE_ASSIGN(Entity);
        };

    }

}

#endif

