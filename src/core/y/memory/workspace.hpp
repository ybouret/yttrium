//! \file

#ifndef Y_Memory_Workspace_Included
#define Y_Memory_Workspace_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"

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
        class Workspace
        {
        public:
            //__________________________________________________________________
            //
            //
            // Modulo
            //
            //__________________________________________________________________

            //! create empty
            inline explicit Workspace() noexcept : data(0), wksp() { Y_STATIC_ZARR(wksp); }

            //! cleanup
            inline virtual ~Workspace() noexcept { erase(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build with default argument
            inline T & build() {
                erase();
                return *( data = new ( Memory::OutOfReach::Addr(wksp) ) T() );
            }

            //! build with one argument
            template <typename U>
            inline T &build(U &args) {
                erase();
                return *( data = new ( Memory::OutOfReach::Addr(wksp) ) T(args) );
            }

            //! dismiss content, assuming it was copied elsewhere
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


