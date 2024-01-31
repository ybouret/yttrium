


//! \file

#ifndef Y_Memory_Embedding_Trio_Included
#define Y_Memory_Embedding_Trio_Included 1

#include "y/memory/embedding/repo.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class EmbeddingTrio : public Embedding::Repo<3>
        {
        public:
            virtual ~EmbeddingTrio() noexcept;

            template <typename T, typename U, typename V>
            inline EmbeddingTrio(T * &p1, const size_t n1,
                                 U * &p2, const size_t n2,
                                 V * &p3, const size_t n3) noexcept :
            Embedding::Repo<3>()
            {
                push(p1,n1);
                push(p2,n2);
                push(p3,n3);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EmbeddingTrio);
        };

    }
}

#endif
