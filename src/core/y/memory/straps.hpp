//! \file

#ifndef Y_Memory_Straps_Included
#define Y_Memory_Straps_Included 1

#include "y/memory/strap.hpp"
#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Straps of Block
        //
        //
        //______________________________________________________________________
        class Straps : public ListOf<Strap>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Memory::Straps"
            typedef void * (Straps:: *Proc)(size_t &);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Straps(Album &userAlbum) noexcept; //!< setup with persistent Album
            virtual ~Straps()                 noexcept; //!< cleanup

            void *acquire(size_t &blockSize);
            void  release(void *blockAddr) noexcept;
            

            
        private:
            Proc    hProc;
            Strap  *cache;
            Strap  *empty;
            Album  &album;

            Y_DISABLE_COPY_AND_ASSIGN(Straps);
            void * acquireIni(size_t &);
            void * acquireAny(size_t &);

        };
    }

}

#endif

