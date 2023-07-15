
//! \file


#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/ilog2.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{
    namespace Apex
    {
        namespace Nexus
        {
            //__________________________________________________________________
            //
            //
            //! Common methods for Apex::Block
            //
            //__________________________________________________________________
            struct Block
            {
                static const size_t   MinBytes = 2*sizeof(uint64_t);
                static const unsigned MinShift = iLog2<MinBytes>::Value;
                static const size_t   MaxBytes = Base2<size_t>::MaxPowerOfTwo;
                static const unsigned MaxShift = Base2<size_t>::MaxShift;

                static unsigned   ShiftFor(const size_t usrBytes);
                static unsigned   ShiftInc(unsigned shift);


                static uint32_t Hash32(const void *, const size_t) noexcept;         //!< helper
                static void *   Acquire(unsigned &shift);                            //!< forward to Archon
                static void     Release(void *entry, const unsigned shift) noexcept; //!< forward to Archon
                

            };
        }

        typedef Int2Type<1>        IncreaseSize_; //!< alias
        extern const IncreaseSize_ IncreaseSize;  //!< alias


#define Y_APEX_BLOCK_CTOR(SHIFT)                                      \
shift( SHIFT ),                                                       \
entry( static_cast<WORD*>(Nexus::Block::Acquire( Coerce(shift) ) ) ), \
bytes( Base2<size_t>::One << shift ),                                 \
words( bytes >> WordShift )

        //______________________________________________________________________
        //
        //
        //
        //! Handling power-of-two sized blocks of power-of-to sized data
        //
        //
        //______________________________________________________________________
        template <typename WORD>
        class Block
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t   WordBytes    = sizeof(WORD); //!< alias
            static const unsigned WordShift    = iLog2<WordBytes>::Value;
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Block(size_t usrBytes) :
            Y_APEX_BLOCK_CTOR(Nexus::Block::ShiftFor(usrBytes))
            {
            }

            explicit Block(const Block &other) :
            Y_APEX_BLOCK_CTOR(other.shift)
            {
                assert(shift>=other.shift);
                assert(bytes>=other.bytes);
                assert(words>=other.words);
                memcpy(entry,other.entry,other.bytes);
            }

            explicit Block(const Block &other, const IncreaseSize_ &) :
            Y_APEX_BLOCK_CTOR( Nexus::Block::ShiftInc(other.shift) )
            {
                assert(shift>=other.shift);
                assert(bytes>=other.bytes);
                assert(words>=other.words);
                memcpy(entry,other.entry,other.bytes);
            }


            virtual ~Block() noexcept { Nexus::Block::Release(entry,shift); }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned shift;
            WORD  * const  entry;
            const size_t   bytes;
            const size_t   words;

        private:
            Y_DISABLE_ASSIGN(Block);
        };
    }

}

#endif

