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
        //______________________________________________________________________
        //
        //
        //! Anonymous Generic Page
        //
        //______________________________________________________________________
        struct Page
        {
            static const size_t DefaultBytes = 4096;                       //!< most common page size
            static const size_t DefaultShift = iLog2<DefaultBytes>::Value; //!< Log2(DefaultBytes)
            static size_t       QueryOsBytes() noexcept;                   //!< GetPageSize() from os

            Page *next; //!< for List
            Page *prev; //!< for List
        };

        //______________________________________________________________________
        //
        //
        //
        //! Low-Level cached pages
        //
        //
        //______________________________________________________________________
        class Pages : public ListOf<Page>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned     useShift  = 1+iLog2Of<Page>::Value;                    //!< minimal meaningful shift
            static const unsigned     usrShift  =     iLog2<256> ::Value;                    //!< practical shift
            static const unsigned     MinShift  = usrShift > useShift ? usrShift : useShift; //!< minimal shift
            static const size_t       MinBytes  = Base2<size_t>::One << MinShift;            //!< minimal page bytes
            static const unsigned     MaxShift  = Base2<size_t>::MaxShift;                   //!< maximal page shift
            static const size_t       MaxBytes  = Base2<size_t>::One << MaxShift;            //!< maximal page bytes
            static const size_t       Number    = 1+MaxShift - MinShift;                     //!< number of possible Pages
            static const char * const CallSign;                                              //!< "Memory::Pages"


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Pages(const unsigned userShift, Lockable &) noexcept; //!< setup with user's shift and giant mutex
            virtual ~Pages()                                     noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void *   acquire();                              //!< acquire with a protected calloc
            void     release(void *) noexcept;               //!< release with a protected free
            void     reserve(size_t n);                      //!< prepare some pages
            void *   query();                                //!< pop/acquire
            void     store(void*) noexcept;                  //!< store ordered by memory
            uint64_t displayInfo(const size_t indent) const; //!< display statistics, return involved memory

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned  shift; //!< bits shift
            const size_t    bytes; //!< bytes = 2^shift
            const ptrdiff_t delta; //!< acquire-release bookkeeping
            Lockable       &giant; //!< giant mutex

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pages);
        };

    }

}

#endif


