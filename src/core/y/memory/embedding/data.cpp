#include "y/memory/embedding/data.hpp"

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            Data:: ~Data() noexcept
            {
                for(size_t i=0;i<size;++i)
                    data[i].dismiss();
                Coerce(size) = 0;
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

            Embed * Data:: head() noexcept
            {
                assert(0!=data);
                return data;
            }

        }
    }

}

