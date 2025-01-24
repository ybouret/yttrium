
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Natural:: shl(const size_t nbit) const
        {
            const size_t bits = block->bits;
            if(nbit<=0||bits<=0)
            {
                return Natural(*this);
            }
            else
            {
                assert( Factory::Exists() );
                static Factory & factory  = Factory::Location();
                const size_t     newBits  = nbit+bits;
                Block * const    newBlock = factory.queryBytes( Y_ALIGN8(newBits) >> 3 );

                {
                    volatile AutoLock lck(*this);
                    block->to(Plan1);
                    newBlock->to(Plan1);
                    for(size_t source=0,target=nbit;source<bits;++source,++target)
                    {
                        if( block->get_(source) ) newBlock->set_(target);
                    }

                }

                newBlock->sync();
                return Natural( newBlock, AsBlock );
            }
        }

        Natural operator<<(const Natural &lhs, const size_t n)
        {
            return lhs.shl(n);
        }

        Natural & Natural:: operator <<= ( const size_t n)
        {
            {
                Natural tmp = shl(n);
                xch(tmp);
            }
            return *this;
        }

    }

}
