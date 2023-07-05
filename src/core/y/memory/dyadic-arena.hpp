//! \file

#ifndef Y_Memory_Dyadic_Arena_Included
#define Y_Memory_Dyadic_Arena_Included 1

#include "y/memory/dyad.hpp"
#include "y/memory/arena.hpp"


namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Arena with a power-of-two blockSize
        //
        //
        //______________________________________________________________________
        class DyadicArena : public Dyad, public Arena
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with 2^userShift blockSize
            explicit DyadicArena(const unsigned  userShift,
                                 Album        &userDataPages,
                                 const size_t  userPageBytes);

            //! cleanup
            virtual ~DyadicArena() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void *acquire();                 //!< acquireBlock
            virtual void  release(void *) noexcept;  //!< releaseBlock
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DyadicArena);
        };

    }

}

#endif

