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
            //
            //! Common methods and parameters for Apex::Block
            //
            //
            //__________________________________________________________________
            struct Block
            {
                static const size_t   MinBytes = 2*sizeof(uint64_t);           //!< at least 128 bits
                static const unsigned MinShift = iLog2<MinBytes>::Value;       //!< Log2(MinBytes)
                static const size_t   MaxBytes = Base2<size_t>::MaxPowerOfTwo; //!< alias
                static const unsigned MaxShift = Base2<size_t>::MaxShift;      //!< alias

                static unsigned   ShiftFor(const size_t usrBytes); //!< Log2(NextPowerOfTwo(usrBytes)) with limits
                static unsigned   ShiftInc(unsigned shift);        //!< shift+1 with limits

                static void *     Acquire(unsigned &shift);                            //!< forward to Archon
                static void       Release(void *entry, const unsigned shift) noexcept; //!< forward to Archon
                

            };
        }

        typedef Int2Type<1>        IncreaseSize_; //!< alias
        extern const IncreaseSize_ IncreaseSize;  //!< alias
        typedef Int2Type<0>        NoMemoryCopy_; //!< alias
        extern const NoMemoryCopy_ NoMemoryCopy;  //!< alias

        //______________________________________________________________________
        //
        //
        //! helper to construct a block with a given SHIFT
        //
        //______________________________________________________________________
#define Y_APEX_BLOCK_CTOR(SHIFT)                                      \
shift( SHIFT ),                                                       \
entry( static_cast<WORD*>(Nexus::Block::Acquire( Coerce(shift) ) ) ), \
bytes( Base2<size_t>::One << shift ),                                 \
words( bytes >> WordShift )

        //______________________________________________________________________
        //
        //
        //
        //! Handling blocks for Proto
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
            static const size_t   WordBytes    = sizeof(WORD);            //!< alias
            static const unsigned WordShift    = iLog2<WordBytes>::Value; //!< alis
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup for usrBytes BYTES
            explicit Block(size_t usrBytes) :
            Y_APEX_BLOCK_CTOR(Nexus::Block::ShiftFor(usrBytes))
            {
            }

            //! copy with copy
            explicit Block(const Block &other) :
            Y_APEX_BLOCK_CTOR(other.shift)
            {
                assert(shift>=other.shift);
                assert(bytes>=other.bytes);
                assert(words>=other.words);
                memcpy(entry,other.entry,other.bytes);
            }

            //! copy WITHOUT data copu
            explicit Block(const Block &other, const NoMemoryCopy_ &) :
            Y_APEX_BLOCK_CTOR(other.shift)
            {
                assert(shift>=other.shift);
                assert(bytes>=other.bytes);
                assert(words>=other.words);
            }


            //! copy with increasing capacity
            explicit Block(const Block &other, const IncreaseSize_ &) :
            Y_APEX_BLOCK_CTOR( Nexus::Block::ShiftInc(other.shift) )
            {
                assert(shift>=other.shift);
                assert(bytes>=other.bytes);
                assert(words>=other.words);
                memcpy(entry,other.entry,other.bytes);
            }

            //! cleanup
            virtual ~Block() noexcept { Nexus::Block::Release(entry,shift); }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned shift; //!< bytes = 2^shift
            WORD  * const  entry; //!< memory
            const size_t   bytes; //!< bytes = 2^shift
            const size_t   words; //!< words wrapping bytes

        private:
            Y_DISABLE_ASSIGN(Block);
        };
    }

}

#endif

