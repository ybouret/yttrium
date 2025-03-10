
//! \file


#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        class Entity
        {
        public:

            explicit Entity(const String *_) noexcept : name(_) {}
            explicit Entity(const XName & _) noexcept : name(_) {}
            virtual ~Entity()      noexcept;

            const String & key() const noexcept;




            const XName name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };
    }
}

#endif
