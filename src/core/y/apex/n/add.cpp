#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"

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
                        const size_t           rn)
            {
                Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidPlan);
                return (ln>=rn) ? Impl(lw,ln,rw,rn) : Impl(rw,rn,lw,ln);
            }

        private:
            static inline
            Block * Impl(const WordType * const bigWord,
                         const size_t           bigSize,
                         const WordType * const litWord,
                         const size_t           litSize)
            {
                static Factory &factory = Factory::Instance();
                const size_t    resSize = bigSize+1;
                Block * const   block   = factory.queryBytes( resSize * sizeof(WordType) );
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
                return block;
            }

        };



    }

}
