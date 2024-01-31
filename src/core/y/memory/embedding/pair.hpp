

//! \file

#ifndef Y_Memory_Embedding_Pair_Included
#define Y_Memory_Embedding_Pair_Included 1

#include "y/memory/embedding/repo.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Two Embeds
        //
        //
        //______________________________________________________________________
        class EmbeddingPair: public Embedding::Repo<2>
        {
        public:
            //! cleanup
            virtual ~EmbeddingPair() noexcept;

            //! setup
            template <typename T, typename U>
            inline EmbeddingPair( T * &p1, const size_t n1,
                                  U * &p2, const size_t n2) noexcept :
            Embedding::Repo<2>()
            {
                push(p1,n1);
                push(p2,n2);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EmbeddingPair);
        };

    }
}

#endif
