#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{
    namespace Apex
    {

        template <
        Plan CORE,
        Plan PLAN
        > struct JigAdd {
            typedef typename Jig<CORE>::Word CoreType;
            typedef          Jig<PLAN>       JigType;
            typedef typename JigType::Word   WordType;
            static const unsigned            WordBits = sizeof(WordType) << 3;

            static inline
            Block * Get(const WordType * const lw,
                        const size_t           ln,
                        const WordType * const rw,
                        const size_t           rn,
                        uint64_t * const       ell)
            {
                Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidPlan);
                return (ln>=rn) ? Impl(lw,ln,rw,rn,ell) : Impl(rw,rn,lw,ln,ell);
            }

        private:
            static inline
            Block * Impl(const WordType * const bigWord,
                         const size_t           bigSize,
                         const WordType * const litWord,
                         const size_t           litSize,
                         uint64_t * const       ell)
            {
                static Factory &factory = Factory::Instance();
                const size_t    resSize = bigSize+1;
                Block * const   block   = factory.queryBytes( resSize * sizeof(WordType) );
                try {
                    const uint64_t mark = (0!=ell) ? WallTime::Ticks() : 0;
                    {
                        JigType  & jig = block->make<PLAN>(); assert(block->curr == &jig);
                        WordType * sum = jig.word;
                        CoreType   acc = 0;

                        for(size_t i=0;i<litSize;++i)
                        {
                            acc   += static_cast<const CoreType>(litWord[i]) + static_cast<const CoreType>(bigWord[i]);
                            sum[i] = static_cast<const WordType>(acc);
                            acc  >>= WordBits;
                        }

                        for(size_t i=litSize;i<bigSize;++i)
                        {
                            acc   += static_cast<const CoreType>(bigWord[i]);
                            sum[i] = static_cast<const WordType>(acc);
                            acc  >>= WordBits;
                        }

                        sum[bigSize] = static_cast<const WordType>(acc);

                    }
                    block->sync();
                    if(0!=ell) *ell += WallTime::Ticks() - mark;
                    return block;
                }
                catch(...)
                {
                    factory.store(block);
                    throw;
                }
            }

        };



    }

}
