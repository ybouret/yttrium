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

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Straps(Album &userAlbum) noexcept; //!< setup with persistent Album
            virtual ~Straps()                 noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            void * acquire(size_t &blockSize);             //!< acquire and update blockSize bytes
            void   release(void   *blockAddr) noexcept;    //!< release previously acquired block
            void   displayInfo(const size_t indent) const; //!< display statistics
            
        private:
            typedef void * (Straps:: *Acquire)(size_t &);

            Acquire  build;
            Strap   *cache;
            Strap   *empty;
            unsigned eshft;
            Album   &album;

            Y_DISABLE_COPY_AND_ASSIGN(Straps);
            void * acquireIni(size_t &);
            void * acquireAny(size_t &);

        };
    }

}

#endif

