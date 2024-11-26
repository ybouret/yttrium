
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/lang.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Entity : public Object
        {
        public:
            explicit Entity() noexcept; //!< topIndex>0
            virtual ~Entity()                      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Entity);

            virtual const String & key() const noexcept = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };

       



    }

}


#endif
