
//! \file

#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/text/justify.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Assembly
        {
        public:
            explicit Assembly() noexcept;
            virtual ~Assembly() noexcept;
            Assembly(const Assembly &) noexcept;

            const size_t maxNameSize;
        private:
            Y_DISABLE_ASSIGN(Assembly);

        };
    }

}

#endif

