
//! \file

#ifndef Y_Memory_Dyad_Included
#define Y_Memory_Dyad_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Dyad
        {
        protected:
            //! 0 <= userShift <= Base2<size_t>::MaxShift
            explicit Dyad(const unsigned userShift) noexcept;

        public:
            //! cleanup
            virtual ~Dyad() noexcept;

            virtual void *acquire()               = 0;
            virtual void  release(void*) noexcept = 0;

            const unsigned shift;
            const size_t   bytes;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dyad);
        };
    }

}

#endif
