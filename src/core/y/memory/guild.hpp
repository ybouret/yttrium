
//! \file

#ifndef Y_Memory_Guild_Included
#define Y_Memory_Guild_Included 1

#include "y/memory/arena.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Specialized Arena
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Guild : public Arena
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup like Arena with sizeof(T)
            inline explicit Guild(Album        &userAlbum) :
            Arena(sizeof(T),userAlbum)
            {
                assert(blockSize>=sizeof(T));
            }

            //! cleanup
            inline virtual ~Guild() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods for dangling items
            //
            //__________________________________________________________________
            inline T   *zombie()                  { return static_cast<T*>(acquireBlock()); }                  //!< create a zombie node
            inline void zstore(T *dead) noexcept  { assert(0!=dead); releaseBlock( OutOfReach::Addr(dead) ); } //!< store a zombie node

            //__________________________________________________________________
            //
            //
            // Methods for consistent items
            //
            //__________________________________________________________________

            //! destruct and release into arena
            inline void eradicate(T * live) noexcept { assert(0!=live); zstore( Destructed(live) ); }

            //! construct with default constructor
            inline T   *construct() {
                void *blockAddr = acquireBlock();
                try { return new (blockAddr) T(); }
                catch(...) {  releaseBlock(blockAddr); throw; }
            }

            //! construct with 1-argument constructor
            template <typename U>
            inline T *construct(U &args) {
                void *blockAddr = acquireBlock();
                try { return new (blockAddr) T(args); }
                catch(...) {  releaseBlock(blockAddr); throw; }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guild);
        };

    }

}

#endif

