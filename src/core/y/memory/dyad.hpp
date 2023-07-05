
//! \file

#ifndef Y_Memory_Dyad_Included
#define Y_Memory_Dyad_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! allocating power-of-two blocks
        //
        //
        //______________________________________________________________________
        class Dyad
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! 0 <= userShift <= Base2<size_t>::MaxShift
            explicit Dyad(const unsigned userShift) noexcept;

        public:
            //! cleanup
            virtual ~Dyad() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            virtual void *acquire()               = 0; //!< acquire a new block
            virtual void  release(void*) noexcept = 0; //!< release a prevuusly acquired block

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned shift; //!< bit shift
            const size_t   bytes; //!< 2^shift


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dyad);
        };
    }

}

#endif
