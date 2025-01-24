
#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural & Natural:: _shr(const size_t nbit) noexcept
        {
            std::cerr << "_shr(" << nbit << ") for #bits=" << block->bits << std::endl;
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
                    const size_t newBits = bits-nbit;
                    size_t       target  = newBits;
                    size_t       source  = bits;
                    std::cerr << "newBits=" << newBits << std::endl;
                    while(target-- > 0 )
                    {
                        if( block->get(--source) )
                        {
                            block->set(target);
                        }
                        else
                        {
                            block->clr_(target);
                        }
                        block->clr_(source);
                    }
                    block->sync();
                    return *this;
                }
            }
        }

    }

}
