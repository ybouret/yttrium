#include "y/memory/embedding/data.hpp"
#include "y/type/destruct.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            Data:: ~Data() noexcept
            {
                // destruct
                while(size>0)
                {
                    Embed &emb = data[ --Coerce(size) ];
                    emb.dismiss();
                    Destruct( &emb );
                }
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

            std::ostream & operator<<(std::ostream &os, const Data &d)
            {
                os << '{' << std::endl;
                for(size_t i=0;i<d.size;++i)
                {
                    os << "  " << d[i] << std::endl;
                }
                os << '}';
                return os;
            }


        }
    }

}

