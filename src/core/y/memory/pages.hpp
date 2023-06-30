//! \file

#ifndef Y_Memory_Pages_Included
#define Y_Memory_Pages_Included 1

#include "y/calculus/ilog2.hpp"
#include "y/calculus/base2.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{

    class Lockable;
    
    namespace Memory
    {

        struct Page
        {
            static const size_t DefaultBytes = 4096;
            static const size_t DefaultShift = iLog2<DefaultBytes>::Value;
            static size_t       QueryOsBytes() noexcept;

            Page *next;
            Page *prev;
        };


        class Pages : public ListOf<Page>
        {
        public:
            static const size_t MinShift = iLog2Of<Page>::Value;
            static const size_t MaxShift = Base2<size_t>::MaxShift;

            Pages(const size_t userShift);
            ~Pages() noexcept;

            const size_t shift; //!< bit shift
            const size_t bytes; //!< bytes = 2^shift

            void *acquire();
            void  release(void *) noexcept;

            void *query();
            void  store(void*) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pages);
            Lockable &access;
        };

    }

}

#endif


