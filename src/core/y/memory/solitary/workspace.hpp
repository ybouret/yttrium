//! \file

#ifndef Y_Memory_Workspace_Included
#define Y_Memory_Workspace_Included 1

#include "y/memory/solitary.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Local memory for single object
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Workspace : public Solitary<T>
        {
        public:
            using Solitary<T>::erase;

            //! create empty
            inline explicit Workspace() noexcept : Solitary<T>(), wksp() { Y_STATIC_ZARR(wksp); }

            //! cleanup
            inline virtual ~Workspace() noexcept { erase();  Y_STATIC_ZARR(wksp); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
            void * wksp[Y_WORDS_FOR(T)];

            inline virtual void *get() noexcept { return Memory::OutOfReach::Addr(wksp); }
        };

    }

}

#endif


