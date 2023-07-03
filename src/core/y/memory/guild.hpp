
//! \file

#ifndef Y_Memory_Guild_Included
#define Y_Memory_Guild_Included 1

#include "y/memory/arena.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Memory
    {


        //! arena facade
        template <typename T>
        class Guild : public Arena
        {
        public:
            inline explicit Guild(Album        &userDataPages,
                                  const size_t  userPageBytes) :
            Arena(sizeof(T),userDataPages,userPageBytes)
            {
                assert(blockSize>=sizeof(T));
            }

            inline virtual ~Guild() noexcept {}

            inline T   *zombie()                  { return static_cast<T*>(acquire); }
            inline void zstore(T *dead) noexcept  { assert(0!=dead);  release( OutOfReach::Addr(dead) ); }

            inline void eradicate(T * &live) noexcept { assert(0!=live); zstore( Destructed(live) ); live=0; }
            inline T   *construct() {
                void *blockAddr = acquire();
                try { return new (blockAddr) T(); }
                catch(...) {  release(blockAddr); throw; }
            }

            template <typename U>
            inline T *construct(U &args) {
                void *blockAddr = acquire();
                try { return new (blockAddr) T(args); }
                catch(...) {  release(blockAddr); throw; }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guild);
        };

    }

}

#endif

