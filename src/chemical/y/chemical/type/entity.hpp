
//! \file


#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_SHALLOW_DECL(AsName);

        class Entity
        {
        public:

            //! XNAME = String *, XName &
            template <typename XNAME> inline
            explicit Entity(const XNAME &_, const AsName_ &) noexcept : name(_)
            {
            }

            Entity(const Entity &) noexcept;

            virtual ~Entity()      noexcept;

            const String & key() const noexcept;

            const XName name;

        private:
            Y_DISABLE_ASSIGN(Entity);
        };
    }
}

#endif
