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
                    Y_STATIC_CHECK(sizeof(WordType)<sizeof(CoreType),InvalidAddPlan);
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
                            
                            const bool     watch = 0!=ell;
                            const uint64_t mark  = watch ? WallTime::Ticks() : 0;
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
                            if(watch) *ell += WallTime::Ticks() - mark;
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
            
        }

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

        Block * Natural:: Add(Block &lhs, Block &rhs)
        {
            return Add(lhs,rhs,AddOpsPrime,0);
        }

        Block * Natural:: Add(Block &lhs, natural_t rhs) {
            typedef Jig4::Word Word;
            const   Jig4      &l = lhs.make<Plan4>();
            size_t             w = 0;
            const void * const q = Block::To(Plan4,rhs,w);
            return JigAdd<Plan8,Plan4>::Get(l.word,l.words,static_cast<const Word *>(q),w,0);
        }


        Natural operator+(const Natural &lhs, const Natural &rhs)
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Add(*lhs.block,*rhs.block), AsBlock );
        }
        
        Natural operator+(const Natural &lhs, const natural_t rhs)
        {
            volatile Natural::AutoLock L(lhs);
            return Natural( Natural::Add(*lhs.block,rhs), AsBlock );
        }

        Natural operator+(const natural_t lhs, const Natural  &rhs)
        {
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Add(*rhs.block,lhs), AsBlock );
        }

        Natural & Natural:: operator+=(const Natural &rhs)
        {
            volatile Natural::AutoLock R(rhs);
            {
                BlockPtr res( Natural::Add(*block,*rhs.block) );
                block.swp(res);
            }
            return *this;
        }

        
        Natural & Natural:: operator+=(const natural_t rhs)
        {
            BlockPtr res( Natural::Add(*block,rhs) );
            block.swp(res);
            return *this;
        }

        void Natural:: incr()
        {
            typedef Jig4::Word Word;
            static const Word rhs = 0x01;
            Jig4             &lhs = block->make<Plan4>();
            BlockPtr res(  JigAdd<Plan8,Plan4>::Get(lhs.word,lhs.words,&rhs,1,0) );
            block.swp(res);
        }

        Natural & Natural:: operator++()
        {
            incr();
            return *this;
        }

        Natural Natural:: operator++(int)
        {
            const Natural old(*this);
            incr();
            return old;
        }

    }

}
