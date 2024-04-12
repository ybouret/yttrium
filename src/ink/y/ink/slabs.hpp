
//! \file

#ifndef Y_Ink_Slabs_Included
#define Y_Ink_Slabs_Included 1

#include "y/concurrent/loop/simt.hpp"
#include "y/ink/slab.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Slabs to setup tessellation for a given area
        //
        //
        //______________________________________________________________________
        class Slabs : public Concurrent::SIMT<Slab>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Slabs(const Concurrent::SharedLoop &);
            virtual ~Slabs() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! computing slabs
            void split(const Area &area);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };
    }

}

#endif

