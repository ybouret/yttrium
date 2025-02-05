
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
            > struct JigMul {
                typedef typename Jig<CORE>::Word CoreType;
                typedef          Jig<PLAN>       JigType;
                typedef typename JigType::Word   WordType;
                static const unsigned            WordSize = sizeof(WordType);
                static const unsigned            WordBits = WordSize << 3;

                static inline
                Block * Get(const WordType * const a,
                            const size_t           p,
                            const WordType *       b,
                            const size_t           q)
                {
                    Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidMulPlan);
                    static Factory & factory = Factory::Instance();

                    if(p<=0||q<=0) return factory.query(0);
                    Block * const  block   = factory.queryBytes( (p+q) * WordSize );
                    WordType *     product = block->make<PLAN>().word;
                    for(size_t j=q;j>0;--j,++product)
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

        Block * Natural:: Mul(Block &lhs, Block &rhs, const Ops mulOps)
        {
            switch(mulOps)
            {
                case Ops2_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigMul<Plan2,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Ops4_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigMul<Plan4,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Ops8_1: {
                    const Jig1 &l = lhs.make<Plan1>();
                    const Jig1 &r = rhs.make<Plan1>();
                    return JigMul<Plan8,Plan1>::Get(l.word,l.words,r.word,r.words);
                }

                case Ops4_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigMul<Plan4,Plan2>::Get(l.word,l.words,r.word,r.words);
                }

                case Ops8_2: {
                    const Jig2 &l = lhs.make<Plan2>();
                    const Jig2 &r = rhs.make<Plan2>();
                    return JigMul<Plan8,Plan2>::Get(l.word,l.words,r.word,r.words);
                }

                case Ops8_4: {
                    const  Jig4 &l = lhs.make<Plan4>();
                    const  Jig4 &r = rhs.make<Plan4>();
                    return JigMul<Plan8,Plan4>::Get(l.word,l.words,r.word,r.words);
                }

            }
            throw Specific::Exception(CallSign, "corrupted mulOps");
            return 0;
        }

        Block * Natural:: Mul(Block &lhs, Block & rhs)
        {
            return Mul(lhs,rhs,MulOps);
        }


        Block * Natural:: Mul(Block &lhs, natural_t rhs)
        {
            typedef Jig4::Word Word;
            const   Jig4      &l = lhs.make<Plan4>();
            size_t             w = 0;
            const void * const q = Block::To(Plan4,rhs,w);
            return JigMul<Plan8,Plan4>::Get(l.word,l.words,static_cast<const Word *>(q),w);
        }


        Natural operator*(const Natural &lhs, const Natural &rhs)
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Mul(*lhs.block,*rhs.block), AsBlock );
        }

        Natural operator*(const Natural &lhs, const natural_t rhs)
        {
            volatile Natural::AutoLock L(lhs);
            return Natural( Natural::Mul(*lhs.block,rhs), AsBlock );
        }

        Natural operator*(const natural_t lhs, const Natural  &rhs)
        {
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Mul(*rhs.block,lhs), AsBlock );
        }

        Natural & Natural:: operator*=(const Natural &rhs)
        {
            volatile Natural::AutoLock R(rhs);
            {
                BlockPtr res( Natural::Mul(*block,*rhs.block) );
                block.swp(res);
            }
            return *this;
        }


        Natural & Natural:: operator*=(const natural_t rhs)
        {
            BlockPtr res( Natural::Mul(*block,rhs) );
            block.swp(res);
            return *this;
        }
    }

}


