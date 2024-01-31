//! \file

#ifndef Y_Memory_Embedding_Quad_Included
#define Y_Memory_Embedding_Quad_Included 1

#include "y/memory/embedding/repo.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Four Embeds
        //
        //
        //______________________________________________________________________
        class EmbeddingQuad : public Embedding::Repo<4>
        {
        public:
            //! cleanup
            virtual ~EmbeddingQuad() noexcept;

            //! setup
            template <typename T, typename U, typename V, typename W>
            inline EmbeddingQuad(T * &p1, const size_t n1,
                                 U * &p2, const size_t n2,
                                 V * &p3, const size_t n3,
                                 W * &p4, const size_t n4) noexcept :
            Embedding::Repo<4>()
            {
                push(p1,n1);
                push(p2,n2);
                push(p3,n3);
                push(p4,n4);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EmbeddingQuad);
        };

    }
}

#endif
