
//! \file

#ifndef Y_Memory_Embedding_Repo_Included
#define Y_Memory_Embedding_Repo_Included 1

#include "y/memory/embedding/room.hpp"
#include "y/memory/embedding/data.hpp"

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
            //! static Embed reposotiry
            //
            //
            //__________________________________________________________________
            template <size_t N>
            class Repo : public Room<N>, public Data
            {
            public:
                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                inline virtual size_t maxi() const noexcept { return N; } //!< maxi=N

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! cleanup
                inline virtual ~Repo() noexcept {}

            protected:

                //! setup
                inline explicit Repo() noexcept : Room<N>(), Data(this->room()) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repo);
            };
        }
    }

}

#endif

