
//! \file

#ifndef Y_Memory_Object_Locator_Included
#define Y_Memory_Object_Locator_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class Arena;
        class Chunk;
        class Strap;

        class ObjectLocator
        {
        public:
            explicit ObjectLocator(const void *blockAddr);
            virtual ~ObjectLocator() noexcept;
            Y_OSTREAM_PROTO(ObjectLocator);

            const void  * where;
            const Arena * arena;
            const Chunk * chunk;
            const Strap * strap;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectLocator);
        };

    }

}

#endif
