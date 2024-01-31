
//! \file

#ifndef Y_Memory_Embedding_Data_Included
#define Y_Memory_Embedding_Data_Included 1

#include "y/memory/embed.hpp"
#include <new>

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            class  Data
            {
            public:
                const size_t        size;
                const Embed * const data;

                virtual size_t maxi() const noexcept = 0;

                virtual ~Data() noexcept;
            protected:
                explicit Data(const void *ptr) noexcept;

                template <typename T> inline
                void push(T * &entry, const size_t count) noexcept
                {
                    assert(size<maxi());
                    const Embed &here = data[Coerce(size)++];
                    new ( (void *) &here ) Embed(entry,count);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Data);
            };

        }

    }

}

#endif

