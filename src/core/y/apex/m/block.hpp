
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
            //__________________________________________________________________
            //
            //
            //! Common methods for Apex::Block
            //
            //__________________________________________________________________
            struct Block
            {
                static void *Acquire(unsigned &shift);                            //!< forward to Archon
                static void  Release(void *entry, const unsigned shift) noexcept; //!< forward to Archon
                static void  TooBigException(const unsigned usrShift,
                                             const unsigned maxShift); //!< raise exception
            };
        }

        //______________________________________________________________________
        //
        //
        //
        // Handling power-of-two sized blocks of power-of-to sized data
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

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Block(const unsigned usrShift) :
            shift( CheckShift(usrShift)+WordShift ),
            entry( static_cast<WORD*>(Nexus::Block::Acquire(Coerce(shift))) ),
            words( (Base2<size_t>::One<<shift) >> WordShift )
            {
            }

            inline virtual ~Block() noexcept { Nexus::Block::Release(entry,shift); }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
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

