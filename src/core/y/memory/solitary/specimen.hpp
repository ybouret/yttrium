
//! \file

#ifndef Y_Memory_Specimen_Included
#define Y_Memory_Specimen_Included 1

#include "y/memory/solitary.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Local allocated memory for single object
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class Specimen : public Solitary<T>
        {
        public:
            inline explicit Specimen() :
            Solitary<T>(),
            wlen(sizeof(T)),
            wksp( ALLOCATOR::Instance().acquire(wlen,1) )
            {
            }

            inline virtual ~Specimen() noexcept {
                ALLOCATOR::Location().release(wksp,wlen);
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Specimen);
            size_t wlen;
            void  *wksp;

            inline virtual void *get() noexcept { assert(0!=wksp); return wksp; }

        };
    }

}

#endif

