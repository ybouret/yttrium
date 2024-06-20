
//! \file

#ifndef Y_Memory_Quarry_Included
#define Y_Memory_Quarry_Included 1

#include "y/data/pool.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/ilog2.hpp"
#include "y/calculus/align.hpp"
#include "y/type/cache.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Dyad;
        class Corpus;
        //______________________________________________________________________
        //
        //
        //
        //! User's level cache of power-of-two sized blocks
        //
        //
        //______________________________________________________________________
        class Quarry : public Cache
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Memory::Quarry"

            //__________________________________________________________________
            //
            //! anonymous block of memory to be used by vein
            //__________________________________________________________________
            struct Stone
            {
                Stone *next; //!< for list/pool
                Stone *prev; //!< for list
            };

            //__________________________________________________________________
            //
            //! Cache of same length blocks
            //__________________________________________________________________
            class Vein : public PoolOf<Stone>, public Releasable
            {
            public:
                static const char * const CallSign;           //!< Memory::Quarry::Vein
                explicit     Vein(Dyad &)   noexcept;         //!< setup from persistent dyad
                virtual     ~Vein()         noexcept;         //!< cleanup
                void *       acquire();                       //!< [query block | acquire block]
                void         release(void*) noexcept;         //!< store previously acquired block
                virtual void release()      noexcept;         //!< release content
                uint64_t     displayInfo(const size_t) const; //!< display statistics
                uint64_t     gc() noexcept;                   //!< order and return total available bytes

                Dyad &dyad; //!< internal allocator with bytes=2^shift
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vein);
                void release_() noexcept;
            };


            static const unsigned MinShift = iLog2Of<Stone>::Value;    //!< alias
            static const unsigned MaxShift = Base2<size_t>::MaxShift;  //!< alias
            static const size_t   Veins    = MaxShift+1-MinShift;      //!< alias
            static const size_t   Required = Veins * sizeof(Vein);     //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Quarry(Corpus &) noexcept; //!< setup from persistent corpus
            virtual ~Quarry()         noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void * acquire(unsigned &shift);                            //!< fetch block 2^(MaxOf(shift,MinShift)), shift<=MaxShift
            void   release(void *entry, const unsigned shift) noexcept; //!< store previously acquire blocks
            void   displayInfo(const size_t) const;                     //!< display statistics

            
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void release() noexcept;
            virtual void gc(const size_t maxBytes) noexcept;




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Quarry);
            Vein *vein;
            void *wksp[Y_WORDS_GEQ(Required)];
        };
    }

}

#endif

