
#include "y/apex/natural.hpp"

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
                    block->ldz();
                    return *this;
                }
                else
                {
                    block->to(Plan1);
                    const size_t newBits = bits - nbit;
                    for(size_t target=0,source=nbit;target<newBits;++target,++source)
                    {
                        if(block->get(source)) {
                            block->set(target);
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

            }
            block->sync();
            assert(bits<=0 || bits-1 == block->bits);
            return *this;
        }

    }

}
