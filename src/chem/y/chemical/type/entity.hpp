
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/lang.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        enum Level
        {
            TopLevel,
            SubLevel,
            AuxLevel
        };

        class Entity : public Object, public Counted
        {
        public:
            static const size_t Levels = AuxLevel + 1;


            explicit Entity(const size_t topIndex) noexcept; //!< topIndex>0
            virtual ~Entity()                      noexcept; //!< cleanup

            virtual const String & key() const noexcept = 0;


            const size_t indx[Levels];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };

    }

}


#endif
