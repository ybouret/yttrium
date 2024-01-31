
#include "y/memory/embedding/data.hpp"

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            Data:: ~Data() noexcept
            {
                Coerce(size) = 0;
                Coerce(data) = 0;
            }
            
            Data:: Data(const void  *ptr,
                        const size_t num) noexcept :
            size(0),
            data( static_cast<const Embed *>(ptr) )
            {
                assert( Good(ptr,num) );
            }
        }
    }

}

