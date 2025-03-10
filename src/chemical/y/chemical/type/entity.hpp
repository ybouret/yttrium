
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

            //! XNAME = String *, XName &
            template <typename XNAME> inline
            explicit Entity(const XNAME &_) noexcept : name(_)
            {
            }

            virtual ~Entity()      noexcept;

            const String & key() const noexcept;




            const XName name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };
    }
}

#endif
