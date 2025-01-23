
#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"
#include "y/system/exception.hpp"

#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {
        template <
        Plan CORE,
        Plan PLAN
        > struct JigSub {
            typedef typename Jig<CORE>::SignedWord CoreType;
            typedef          Jig<PLAN>             JigType;
            typedef typename JigType::Word         WordType;
            static const unsigned                  WordBits = sizeof(WordType) << 3;

            static inline
            Block * Get(const WordType * const lw,
                        const size_t           ln,
                        const WordType * const rw,
                        const size_t           rn)
            {
                Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidSubPlan);
                static const CoreType Radix = IntegerFor<WordType>::Maximum;

                if(rn>ln) throw Libc::Exception(EDOM, "Apex::Sub(lhs<rhs) Level-1");

                static Factory &factory = Factory::Instance();
                Block * const   block   = factory.queryBytes( ln * sizeof(WordType) );

                try
                {
                    JigType  & jig   = block->make<PLAN>(); assert(block->curr == &jig);
                    WordType * dif   = jig.word;
                    CoreType   carry = 0;

                    // common size
                    for(size_t i=0;i<rn;++i)
                    {
                        const CoreType l = static_cast<CoreType>( lw[i] );
                        const CoreType r = static_cast<CoreType>( rw[i] );
                        carry += l-r;
                        if(carry<0)
                        {
                            dif[i] = static_cast<WordType>(carry+Radix);
                            carry  = -1;
                        }
                        else
                        {
                            dif[i] = static_cast<WordType>(carry);
                            carry  = 0;
                        }
                    }

                    // propagate carry
                    for(size_t i=rn;i<ln;++i)
                    {
                        const CoreType l = static_cast<CoreType>( lw[i] );
                        carry += l;
                        if(carry<0)
                        {
                            dif[i] = static_cast<WordType>(carry+Radix);
                            carry  = -1;
                        }
                        else
                        {
                            dif[i] = static_cast<WordType>(carry);
                            carry  = 0;
                        }
                    }

                    if(carry<0) throw Libc::Exception(EDOM, "Assembly::Sub(lhs<rhs) Level-2");


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

        Block * Natural:: Sub(Block &lhs, Block &rhs, const AddOps addOps)
        {
            switch(addOps)
            {
                case Add2_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigSub<Plan2,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Add4_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigSub<Plan4,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Add8_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigSub<Plan8,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Add4_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigSub<Plan4,Plan2>::Get(l.word,l.words,r.word,r.words);
                }

                case Add8_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigSub<Plan8,Plan2>::Get(l.word,l.words,r.word,r.words);
                }

                case Add8_4: {
                    const  Jig4 &l = lhs.make<Plan4>();
                    const  Jig4 &r = rhs.make<Plan4>();
                    return JigSub<Plan8,Plan4>::Get(l.word,l.words,r.word,r.words);
                }

            }
            
            throw Specific::Exception(CallSign, "corrupted addOps");
            return 0;
        }


        Natural operator-(const Natural & lhs, const Natural & rhs)
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(lhs);
            
            return Natural( Natural::Sub(*lhs.block, *rhs.block, Natural::AddOpsPrime), AsBlock );

        }
    }

}

