//! \file

#ifndef Y_Memory_Workspace_Included
#define Y_Memory_Workspace_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Memory
    {

        template <typename T>
        class Workspace
        {
        public:
            inline explicit Workspace() noexcept : data(0), wksp()
            {
                Y_STATIC_ZARR(wksp);
            }

            inline virtual ~Workspace() noexcept { erase(); }

            inline T & build()
            {
                erase();
                data = new ( Memory::OutOfReach::Addr(wksp) ) T();
                return *data;
            }

            template <typename U>
            inline T &build(U &args)
            {
                erase();
                data = new ( Memory::OutOfReach::Addr(wksp) ) T(args);
                return *data;
            }

            inline void dismiss() noexcept { data = 0; Y_STATIC_ZARR(wksp); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
            T    * data;
            void * wksp[Y_WORDS_FOR(T)];

            inline void erase() noexcept {
                if(0!=data)
                {
                    Memory::OutOfReach::Naught(data);
                    data = 0;
                }
            }
        };

    }

}

#endif


