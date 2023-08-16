//! \file

#ifndef Y_Ordered_Core_Heap_Included
#define Y_Ordered_Core_Heap_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! Core Heap to old CallSign
        //
        //
        //______________________________________________________________________
        class Heap
        {
        public:
            static const char * const CallSign; //!< "Heap"
            virtual ~Heap() noexcept;           //!< cleanup
            explicit Heap() noexcept;           //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Heap);
        };
    }

}



#endif
