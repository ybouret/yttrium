
//! \file


#ifndef Y_Apex_Archon_Included
#define Y_Apex_Archon_Included 1

#include "y/singleton.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Apex
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
            static const char * const      CallSign;                                //!< "Apex::Archon"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 5; //!< alias
            class Engine;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void * acquire(unsigned &shift);                            //!< fetch block 2^(MaxOf(shift,MinShift)), shift<=MaxShift
            void   release(void *entry, const unsigned shift) noexcept; //!< store previously acquire blocks



            static unsigned ShiftFor(size_t       &bytes);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Archon);
            explicit Archon() noexcept;
            virtual ~Archon() noexcept;
            friend class Singleton<Archon>;
            static void  CheckRequired(size_t       &bytes,
                                       unsigned     &shift);

        };

        template <typename WORD>
        class Block
        {
        public:
            static const size_t   WordBytes = sizeof(WORD);
            static const unsigned WordShift = iLog2<WordBytes>::Value;
            static const unsigned MaxShift  = 9;

            explicit Block(const size_t numWords),
            bytes()
            {
            }

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
        };
    }

}

#endif
