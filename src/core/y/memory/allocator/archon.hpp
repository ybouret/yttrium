//! \file

#ifndef Y_MEMORY_ARCHON_INCLUDED
#define Y_MEMORY_ARCHON_INCLUDED 1

#include "y/singleton.hpp"

namespace Yttrium
{
    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! singleton for cache of memory blocks
        //
        //
        //______________________________________________________________________
        class Archon : public Singleton<Archon>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                //!< "Memory::Archon"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 5; //!< alias
            static const unsigned          MinShift;                                //!< Quarry::MinShift
            static const size_t            MinBytes;                                //!< 2^MinShift
            class Engine;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void * acquire(unsigned &shift);                            //!< fetch block 2^(MaxOf(shift,MinShift)), shift<=MaxShift
            void   release(void *entry, const unsigned shift) noexcept; //!< store previously acquire blocks

            static void * Acquire(unsigned &shift);                             //!< acquire with internal instance
            static void   Release(void * entry, const unsigned shift) noexcept; //!< release with internal location

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Archon);
            explicit Archon() noexcept;
            virtual ~Archon() noexcept;
            friend class Singleton<Archon>;

        };

    }

}

#endif

