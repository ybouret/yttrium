//! \file

#ifndef Y_Memory_Dyadic_Arena_Included
#define Y_Memory_Dyadic_Arena_Included 1

#include "y/memory/dyad.hpp"
#include "y/memory/arena.hpp"


namespace Yttrium
{

    namespace Memory
    {
        class DyadicArena : public Dyad, public Arena
        {
        public:
            explicit DyadicArena(const unsigned  userShift,
                                 Album        &userDataPages,
                                 const size_t  userPageBytes);

            virtual ~DyadicArena() noexcept;

            virtual void *acquire();
            virtual void  release(void *) noexcept;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(DyadicArena);
        };

    }

}

#endif

