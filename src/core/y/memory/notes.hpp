
//! \file

#ifndef Y_Memory_Notes_Included
#define Y_Memory_Notes_Included 1

#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Notes
        {
        public:
            static const unsigned MinShift = 0;
            static const unsigned MaxShift = Pages::MinShift-1;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Notes);
        };
    }

}
#endif

