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
            static const unsigned     useShift  = 1+iLog2Of<Page>::Value;
            static const unsigned     usrShift  =     iLog2<256> ::Value;
            static const unsigned     MinShift  = usrShift > useShift ? usrShift : useShift;
            static const size_t       MinBytes  = Base2<size_t>::One << MinShift;
            static const unsigned     MaxShift  = Base2<size_t>::MaxShift;
            static const size_t       MaxBytes  = Base2<size_t>::One << MaxShift;
            static const size_t       Number    = 1+MaxShift - MinShift;
            
            static const char * const CallSign;
            
            explicit Pages(const unsigned userShift,
                           Lockable      &userMutex) noexcept;
            virtual ~Pages() noexcept;

            void *acquire();                //!< acquire with a protected calloc
            void  release(void *) noexcept; //!< release with a protected free
            void  reserve(size_t n);        //!< prepare some pages

            void *query();
            void  store(void*) noexcept;

            uint64_t displayInfo(const size_t indent) const;


            const unsigned  shift; //!< bit shift
            const size_t    bytes; //!< bytes = 2^shift
            const ptrdiff_t delta; //!< acquire-release bookkeeping
            Lockable       &giant; //!< giant mutex

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pages);
        };

    }

}

#endif


