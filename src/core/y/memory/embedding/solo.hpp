
//! \file

#ifndef Y_Memory_Embedding_Solo_Included
#define Y_Memory_Embedding_Solo_Included 1

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
            //! One Embed
            //
            //
            //__________________________________________________________________
            class Solo : public Repo<1>
            {
            public:
                //! cleanup
                virtual ~Solo() noexcept;

                //! setup empty
                explicit Solo() noexcept;
                
                //! setup with data
                template <typename T>
                inline explicit Solo( T * &p1, const size_t n1) noexcept :
                Repo<1>() {
                    push(p1,n1);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Solo);
            };

        }
    }
}

#endif
