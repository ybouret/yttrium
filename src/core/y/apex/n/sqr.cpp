
#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"
#include "y/system/wtime.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace
        {

            template <
            Plan CORE,
            Plan PLAN
            > struct JigSqr {
                typedef typename Jig<CORE>::Word CoreType;
                typedef          Jig<PLAN>       JigType;
                typedef typename JigType::Word   WordType;
                static const unsigned            WordSize = sizeof(WordType);
                static const unsigned            WordBits = WordSize << 3;

                static inline
                Block * Get(const WordType * const a,
                            const size_t           p)
                {
                    Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidMulPlan);
                    static Factory & factory = Factory::Instance();

                    if(p<=0) return factory.query(0);
                    Block * const    block   = factory.queryBytes( (p+p) * WordSize );
                    WordType *       product = block->make<PLAN>().word;
                    const WordType * b       = a;
                    for(size_t j=p;j>0;--j,++product)
                    {
                        const CoreType B     = static_cast<CoreType>(*(b++));
                        CoreType       carry = 0;
                        for(size_t i=0;i<p;++i)
                        {
                            carry     += static_cast<CoreType>(product[i]) + static_cast<CoreType>(a[i]) * B;
                            product[i] = static_cast<WordType>(carry);
                            carry >>= WordBits;
                        }
                        product[p] =  static_cast<WordType>(carry);
                    }
                    block->sync();
                    return block;

                }

            };

        }

        Block * Natural:: BFA_Sqr(Block &lhs)
        {
            switch(Natural::MulOps)
            {
                case Ops2_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    return JigSqr<Plan2,Plan1>::Get(l.word,l.words);
                }

                case Ops4_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    return JigSqr<Plan4,Plan1>::Get(l.word,l.words);
                }

                case Ops8_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    return JigSqr<Plan8,Plan1>::Get(l.word,l.words);
                }

                case Ops4_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    return JigSqr<Plan4,Plan2>::Get(l.word,l.words);
                }

                case Ops8_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    return JigSqr<Plan8,Plan2>::Get(l.word,l.words);
                }

                case Ops8_4: {
                    const  Jig4 &l = lhs.make<Plan4>();
                    return JigSqr<Plan8,Plan4>::Get(l.word,l.words);
                }

            }
            throw Specific::Exception(Natural::CallSign, "corrupted MulOps");
            return 0;
        }
        
        Natural Natural:: sqr() const
        {
            volatile AutoLock I_lock(*this);
            return Natural( BFA_Sqr(*block), AsBlock );
        }
    }

}

