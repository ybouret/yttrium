
//! \file

#ifndef Y_Memory_Embedding_Solo_Included
#define Y_Memory_Embedding_Solo_Included 1

#include "y/memory/embedding/repo.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! One Embed
        //
        //
        //______________________________________________________________________
        class EmbeddingSolo : public Embedding::Repo<1>
        {
        public:
            //! cleanup
            virtual ~EmbeddingSolo() noexcept;

            //! setup
            template <typename T>
            inline EmbeddingSolo( T * &p1, const size_t n1) noexcept :
            Embedding::Repo<1>()
            {
                push(p1,n1);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EmbeddingSolo);
        };

    }
}

#endif
