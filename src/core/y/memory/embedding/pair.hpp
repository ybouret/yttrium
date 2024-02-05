

//! \file

#ifndef Y_Memory_Embedding_Pair_Included
#define Y_Memory_Embedding_Pair_Included 1

#include "y/memory/embedding/repo.hpp"

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            //__________________________________________________________________
            //
            //
            //
            //! Two Embeds
            //
            //
            //__________________________________________________________________
            class Pair : public Repo<2>
            {
            public:
                //! cleanup
                virtual ~Pair() noexcept;


                //! setup empty
                inline explicit Pair() noexcept;

                //! setup
                template <typename T, typename U>
                inline explicit Pair(T * &p1, const size_t n1,
                                     U * &p2, const size_t n2) noexcept :
                Repo<2>() {
                    push(p1,n1);
                    push(p2,n2);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pair);
            };
        }

    }
}

#endif
