//! \file

#ifndef Y_Memory_Straps_Included
#define Y_Memory_Straps_Included 1

#include "y/memory/strap.hpp"
#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Straps : public ListOf<Strap>
        {
        public:
            explicit Straps(Album &userAlbum) noexcept;
            virtual ~Straps() noexcept;
            

            
        private:
            Strap  *cache;
            Strap  *empty;
            Album  &album;
            
            Y_DISABLE_COPY_AND_ASSIGN(Straps);

        };
    }

}

#endif

