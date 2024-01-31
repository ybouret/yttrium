
//! \file

#ifndef Y_Memory_Embedding_Included
#define Y_Memory_Embedding_Included 1

#include "y/memory/embed.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class EmbeddingData
        {
        public:

        private:
            
        };

        template <size_t N>
        class Embedding
        {
        public:
            static const size_t Length = N * sizeof(Embed);
            inline virtual ~Embedding() noexcept {}

            inline size_t        size() const noexcept { return built; }
            inline const Embed * data() const noexcept { return embed; }

            template <typename T>
            inline void push(T * &entry, const size_t count) noexcept
            {
                assert(built<N);
            }


        protected:
            inline explicit Embedding() noexcept :
            built(0),
            embed(0),
            words()
            {
                embed = static_cast<Embed *>( Memory::OutOfReach::Addr(memset(words,0,sizeof(words))) );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Embedding);
            size_t built;
            Embed *embed;
            void  *words[ Y_WORDS_GEQ(Length) ];
        };
    }

}

#endif
