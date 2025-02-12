
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural & Natural:: _shr(const size_t nbit) noexcept
        {
            if(nbit<=0)
            {
                return *this;
            }
            else
            {
                assert(nbit>0);
                const size_t bits = block->bits;
                if(nbit>=bits) {
                    block->ldz(Plan1);
                    return *this;
                }
                else
                {
                    block->to(Plan1);
                    const size_t newBits = bits - nbit;
                    for(size_t target=0,source=nbit;target<newBits;++target,++source)
                    {
                        if(block->get_(source)) {
                            block->set_(target);
                        }
                        else {
                            block->clr_(target);
                        }
                    }

                    for(size_t source=newBits;source<bits;++source)
                        block->clr_(source);


                    block->sync();
                    assert(newBits==block->bits);
                    return *this;
                }
            }
        }

        Natural & Natural:: _shr() noexcept
        {
            const size_t bits = block->bits;
            if(bits<=64)
            {
                Jig8 &jig = block->make<Plan8>(); assert(jig.words<=1);
                jig.word[0] >>= 1;
            }
            else
            {
                Jig1 &          jig = block->make<Plan1>();
                uint8_t * const wrd = jig.word;
                const size_t    msw = jig.words-1;
                for(size_t i=0,j=1;i<msw;++i,++j)
                {
                    uint8_t &w = wrd[i];
                    w >>= 1;
                    if( 0 != (wrd[j] & 0x1) ) w |= 128;
                }
                wrd[msw] >>= 1;
            }
            block->sync();
            assert(bits<=0 || bits-1 == block->bits);
            return *this;
        }


        Natural  Natural:: shr(const size_t nbit) const
        {
            if(nbit<=0) {
                return Natural(*this);
            }
            else
            {
                const size_t bits = block->bits;
                if(nbit>=bits)
                {
                    return Natural(0);
                }
                else
                {
                    assert(Factory::Exists());

                    static Factory & factory = Factory::Location();
                    const size_t     newBits  = bits-nbit;
                    Block *          newBlock = factory.queryBytes( Y_ALIGN8(newBits) >> 3 );
                    
                    {
                        volatile AutoLock lck(*this);
                        block->to(Plan1);
                        newBlock->to(Plan1);
                        for(size_t target=0,source=nbit;target<newBits;++target,++source)
                        {
                            if(block->get_(source))
                                newBlock->set_(target);
                        }
                    }


                    newBlock->sync(); assert(newBits==newBlock->bits);
                    return Natural(newBlock,AsBlock);
                }
            }
        }


        Natural operator>>(const Natural &lhs, const size_t n)
        {
            return lhs.shr(n);
        }

        Natural & Natural:: operator>>=(const size_t n) noexcept
        {
            _shr(n);
            return *this;
        }


    }

}
