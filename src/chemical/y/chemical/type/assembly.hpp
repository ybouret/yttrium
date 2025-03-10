//! \file


#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Assembly
        {
        public:
            explicit Assembly() noexcept;
            virtual ~Assembly() noexcept;

            void enroll(const Entity &);

            const size_t maxNameLength;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };
    }

}

#endif

