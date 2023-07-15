
//! \file


#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/ilog2.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Nexus
        {
            struct Block
            {
                static void TooBigException(const unsigned usrShift,
                                            const unsigned maxShift);

                static void *Acquire(unsigned &shift);
                static void  Release(void *entry, const unsigned shift) noexcept;
            };
        }


        template <typename WORD>
        class Block
        {
        public:
            static const size_t   WordBytes = sizeof(WORD);
            static const unsigned WordShift = iLog2<WordBytes>::Value;
            static const unsigned MaxShift  = Base2<size_t>::MaxShift - WordShift;
            static const size_t   MaxWords  = Base2<size_t>::One << MaxShift;

            inline explicit Block(const unsigned usrShift) :
            shift( CheckShift(usrShift)+WordShift ),
            entry( static_cast<WORD*>(Nexus::Block::Acquire(Coerce(shift))) ),
            words( (Base2<size_t>::One<<shift) >> WordShift )
            {
            }

            inline virtual ~Block() noexcept { Nexus::Block::Release(entry,shift); }

            const unsigned shift;
            WORD * const   entry;
            const size_t   words;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
            static inline unsigned CheckShift(const unsigned usrShift)
            {
                if(usrShift>MaxShift) Nexus::Block::TooBigException(usrShift,MaxShift);
                return usrShift;
            }
        };
    }

}

#endif

