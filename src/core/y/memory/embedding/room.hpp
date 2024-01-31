
//! \file

#ifndef Y_Memory_Embedding_Room_Included
#define Y_Memory_Embedding_Room_Included 1

#include "y/memory/embed.hpp"
#include "y/calculus/align.hpp"
#include <cstring>

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
            //! static room to hold embedding info
            //
            //
            //__________________________________________________________________
            template <size_t N>
            class Room
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const size_t Required = N * sizeof(Embed); //!< required bytes to hold Embed[N]

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! cleanup
                inline virtual ~Room() noexcept { clr(); }
            protected:
                //! setup
                explicit Room() noexcept : wksp() { clr(); }

                //! get address for derived classes
                inline const void *room() const noexcept { return &wksp[0]; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Room);
                void *wksp[ Y_WORDS_GEQ(Required) ];
                inline void clr() noexcept { memset(wksp,0,sizeof(wksp)); }
            };
        }
    }

}

#endif
