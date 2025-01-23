
#include "y/apex/natural.hpp"

#include "y/apex/block/factory.hpp"
#include "y/check/static.hpp"
#include "y/system/exception.hpp"

#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {

        namespace
        {
            template <
            Plan CORE,
            Plan PLAN
            > struct JigSub {
                typedef typename Jig<CORE>::SignedWord CarryType;
                typedef          Jig<PLAN>             JigType;
                typedef typename JigType::Word         WordType;
                static const unsigned                  WordBits = sizeof(WordType) << 3;
                static const CarryType                 Radix    = CarryType(1) << WordBits;
                
                static inline
                Block * Get(const WordType * const lw,
                            const size_t           ln,
                            const WordType * const rw,
                            const size_t           rn)
                {
                    Y_STATIC_CHECK(sizeof(WordType)<sizeof(CarryType),InvalidSubPlan);
                    
                    if(rn>ln)
                        throw Libc::Exception(EDOM, "Apex::Sub(lhs<rhs) Level-1");
                    
                    static Factory &factory = Factory::Instance();
                    Block * const   block   = factory.queryBytes( ln * sizeof(WordType) );
                    
                    try
                    {
                        JigType  &       j   = block->make<PLAN>(); assert(block->curr == &j);
                        WordType * const d   = j.word;
                        CarryType        cr  = 0;
                        
                        //----------------------------------------------------------
                        //
                        // common size
                        //
                        //----------------------------------------------------------
                        for(size_t i=0;i<rn;++i)
                        {
                            cr += static_cast<CarryType>( lw[i] ) - static_cast<CarryType>( rw[i] );
                            if(cr<0)
                            {
                                d[i] = static_cast<WordType>(cr+Radix);
                                cr  = -1;
                            }
                            else
                            {
                                d[i] = static_cast<WordType>(cr);
                                cr  = 0;
                            }
                        }
                        
                        //----------------------------------------------------------
                        //
                        // propagate carry
                        //
                        //----------------------------------------------------------
                        for(size_t i=rn;i<ln;++i)
                        {
                            cr += static_cast<CarryType>( lw[i] );
                            if(cr<0)
                            {
                                d[i] = static_cast<WordType>(cr+Radix);
                                cr   = -1;
                            }
                            else
                            {
                                d[i] = static_cast<WordType>(cr);
                                cr   = 0;
                            }
                        }
                        
                        if(cr<0)
                            throw Libc::Exception(EDOM, "Apex::Sub(lhs<rhs) Level-2");
                        
                        
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
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Sub(*lhs.block, *rhs.block, Natural::AddOpsPrime), AsBlock );
        }

        Block * Natural:: Sub(Block &lhs, natural_t rhs)
        {
            typedef Jig4::Word Word;
            const   Jig4      &l = lhs.make<Plan4>();
            size_t             w = 0;
            const void * const q = Block::To(Plan4,rhs,w);
            return JigSub<Plan8,Plan4>::Get(l.word,l.words,static_cast<const Word *>(q),w);
        }

        Block * Natural:: Sub(natural_t lhs, Block &rhs)
        {
            typedef Jig4::Word Word;
            const   Jig4      &r = rhs.make<Plan4>();
            size_t             w = 0;
            const void * const q = Block::To(Plan4,lhs,w);
            return JigSub<Plan8,Plan4>::Get(static_cast<const Word *>(q),w,r.word,r.words);
        }

        Natural operator-(const Natural & lhs, const natural_t rhs)
        {
            volatile Natural::AutoLock L(lhs);
            return Natural( Natural::Sub(*lhs.block, rhs), AsBlock );
        }

        Natural operator-(const natural_t lhs, const Natural & rhs)
        {
            volatile Natural::AutoLock R(rhs);
            return Natural( Natural::Sub(lhs, *rhs.block), AsBlock );
        }

        Natural & Natural:: operator-=(const Natural & rhs)
        {
            volatile Natural::AutoLock R(rhs);
            {
                BlockPtr tmp( Sub(*block, *rhs.block, Natural::AddOpsPrime ) );
                block.swp(tmp);
            }
            return *this;
        }

        Natural & Natural:: operator-=(const natural_t rhs)
        {
            BlockPtr tmp( Sub(*block,rhs) );
            block.swp(tmp);
            return *this;
        }

        void Natural:: decr()
        {
            typedef Jig4::Word Word;
            static const Word rhs = 0x01;
            Jig4             &lhs = block->make<Plan4>();
            BlockPtr res(  JigSub<Plan8,Plan4>::Get(lhs.word,lhs.words,&rhs,1) );
            block.swp(res);
        }


        Natural & Natural:: operator--()
        {
            decr();
            return *this;
        }

        Natural Natural:: operator--(int)
        {
            const Natural old(*this);
            decr();
            return old;
        }

    }

}

