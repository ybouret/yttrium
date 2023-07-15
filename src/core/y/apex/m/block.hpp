
//! \file


#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/ilog2.hpp"
#include <cstring>

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
                static uint32_t Hash32(const void *, const size_t) noexcept;         //!< helper
                static void *   Acquire(unsigned &shift);                            //!< forward to Archon
                static void     Release(void *entry, const unsigned shift) noexcept; //!< forward to Archon
                static void     TooBigException(const unsigned usrShift,             //
                                                const unsigned maxShift);            //!< raise exception


            };
        }

        typedef Int2Type<1>        IncreaseSize_; //!< alias
        extern const IncreaseSize_ IncreaseSize;  //!< alias

        //! helper for constructor
#define Y_APEX_BLOCK_CTOR(ARGS)                                   \
shift( ARGS ),                                                    \
entry( static_cast<WORD*>(Nexus::Block::Acquire(Coerce(shift))) ), \
bytes( Base2<size_t>::One<<shift ),                                \
words( bytes >> WordShift        )

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
            static const size_t   WordBytes = sizeof(WORD);                        //!< alias
            static const unsigned WordShift = iLog2<WordBytes>::Value;             //!< alias
            static const unsigned MaxShift  = Base2<size_t>::MaxShift - WordShift; //!< alias
            static const size_t   MaxWords  = Base2<size_t>::One << MaxShift;      //!< alias
            static const size_t   MinWords  = (2*sizeof(uint64_t)) >> WordShift;       //!< alias
            static const size_t   MinShift  = iLog2<MinWords>::Value;              //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with >= 2^usrShift words
            inline   Block(const unsigned usrShift) :
            Y_APEX_BLOCK_CTOR( CheckShift(usrShift)+WordShift )
            {
            }

            //! cleanup
            inline   ~Block() noexcept { Nexus::Block::Release(entry,shift); }


            //! copy
            inline Block(const Block &other) :
            Y_APEX_BLOCK_CTOR( other.shift )
            {
                assert(words>=other.words);
                assert(bytes>=other.bytes);
                memcpy(words,other.words,bytes);
            }

            //! copy into increased size
            inline Block(const Block &other, const IncreaseSize_ &) :
            Y_APEX_BLOCK_CTOR( other.shift+1 )
            {
                assert(words>other.words);
                assert(bytes>other.bytes);
                memcpy(words,other.words,bytes);
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned shift; //!< bytes = 2^shift
            WORD * const   entry; //!< entry[0..words-1]
            const size_t   bytes; //!< bytes = 2^shift
            const size_t   words; //!< words = bytes >> WordShift

        private:
            Y_DISABLE_ASSIGN(Block);
            static inline unsigned CheckShift(unsigned usrShift)
            {
                if(usrShift<MinShift) usrShift = MinShift;
                if(usrShift>MaxShift) Nexus::Block::TooBigException(usrShift,MaxShift);
                return usrShift;
            }
        };
    }

}

#endif

