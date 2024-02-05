#include "y/memory/embedding/data.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            Data:: ~Data() noexcept
            {
                while(size>0)
                    Memory::OutOfReach::Naught(&data[ --Coerce(size) ] );
                Coerce(data) = 0;
            }
            
            Data:: Data(void  *ptr) noexcept :
            size(0),
            data( static_cast<Embed *>(ptr) )
            {
            }

            Embed & Data:: operator[](const size_t indx) noexcept
            {
                assert(indx<size);
                return data[indx];
            }

            const Embed & Data:: operator[](const size_t indx) const noexcept
            {
                assert(indx<size);
                return data[indx];
            }

            

        }
    }

}

