
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Entity : public Object, public Counted
        {
        public:

            template <typename NAME> inline
            explicit Entity(const NAME & _name) :
            Object(),
            Counted(),
            name(_name)
            {

            }

            virtual ~Entity() noexcept;

            const String & key() const noexcept;

            const String name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };

    }
}

#endif
