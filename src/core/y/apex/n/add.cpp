#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"
#include "y/system/wtime.hpp"
#include "y/system/exception.hpp"

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
                try
                {
                    {
                        JigType  & jig = block->make<PLAN>(); assert(block->curr == &jig);
                        WordType * sum = jig.word;
                        CoreType   acc = 0;

                        const uint64_t mark = (0!=ell) ? WallTime::Ticks() : 0;
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
                        if(0!=ell) *ell += WallTime::Ticks() - mark;
                    }

                    block->sync();
                    return block;
                }
                catch(...)
                {
                    factory.store(block);
                    throw;
                }
            }

        };


        Block * Natural:: Add(Block &lhs, Block &rhs, const AddOps addOps, uint64_t * const ell)
        {
            switch(addOps)
            {
                case Add2_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigAdd<Plan2,Plan1>::Get(l.word,l.words,r.word,r.words,ell);
                }

                case Add4_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigAdd<Plan4,Plan1>::Get(l.word,l.words,r.word,r.words,ell);
                }

                case Add8_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigAdd<Plan8,Plan1>::Get(l.word,l.words,r.word,r.words,ell);
                }

                case Add4_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigAdd<Plan4,Plan2>::Get(l.word,l.words,r.word,r.words,ell);
                }

                case Add8_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigAdd<Plan8,Plan2>::Get(l.word,l.words,r.word,r.words,ell);
                }

                case Add8_4: {
                    const  Jig4 &l = lhs.make<Plan4>();
                    const  Jig4 &r = rhs.make<Plan4>();
                    return JigAdd<Plan8,Plan4>::Get(l.word,l.words,r.word,r.words,ell);
                }

            }
            throw Specific::Exception(CallSign, "corrupted addOps");
            return 0;
        }

    }

}
