


//! \file

#ifndef Y_Memory_Embedding_Trio_Included
#define Y_Memory_Embedding_Trio_Included 1

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
            //! Three Embeds
            //
            //
            //______________________________________________________________________
            class Trio : public Repo<3>
            {
            public:
                //! cleanup
                virtual ~Trio() noexcept;

                //! setup
                template <typename T, typename U, typename V>
                inline Trio(T * &p1, const size_t n1,
                            U * &p2, const size_t n2,
                            V * &p3, const size_t n3) noexcept :
                Repo<3>() {
                    push(p1,n1);
                    push(p2,n2);
                    push(p3,n3);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Trio);
            };
        }

    }
}

#endif
