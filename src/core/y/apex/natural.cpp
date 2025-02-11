
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Y_SHALLOW_IMPL(AsBlock);
        Y_SHALLOW_IMPL(Exp2);


        Natural:: AutoLock:: AutoLock(const Natural &n) noexcept :
        host(n) {
            host.access().lock();
        }

        Natural:: AutoLock:: ~AutoLock() noexcept {
            host.access().unlock();
        }


        const char * const Natural:: CallSign = "Apex::Natural";
        const char *       Natural:: callSign() const noexcept { return CallSign; }



        Ops Natural:: AddOps = Ops8_4;
        Ops Natural:: MulOps = Ops8_4;


        const Ops Natural:: OpsTable[NumOps] =
        {
            Y_Apex_Ops_List
        };

        const char * const Natural:: OpsLabel[NumOps] =
        {
            "Ops8_1",
            "Ops8_2",
            "Ops8_4",
            "Ops4_1",
            "Ops4_2",
            "Ops2_1"
        };

        static inline Factory & _Factory()
        {
            static Factory & _ = Factory::Instance();
            return _;
        }

        Y_PROXY_IMPL(Natural, *block)


        static const unsigned    NaturalShift = iLog2Of<natural_t>::Value;
        static const Plan        NaturalPlan  = Plan(NaturalShift);


        Natural:: Natural() :
        Castable(),
        block( _Factory().query(0) ),
        mutex( _Factory().query()  )
        {

        }

        Natural:: Natural(const natural_t n) :
        Castable(),
        block( _Factory().query(NaturalShift) ),
        mutex( _Factory().query() )
        {
            block->make<NaturalPlan>().word[0] = n;
            block->sync();
        }

        Natural:: Natural(Random::Bits &ran, const size_t nbit):
        Castable(),
        Proxy<Block>(),
        block( _Factory().query(ran,nbit) ),
        mutex( _Factory().query() )
        {
            assert(nbit==block->bits);
        }

        Natural:: Natural(Block * const userBlock, const AsBlock_ &) :
        Castable(),
        Proxy<Block>(),
        block( userBlock ),
        mutex( _Factory().query() )
        {

        }



        Natural:: ~Natural() noexcept {

        }

        Natural:: Natural(const Natural &other) :
        Castable(),
        Proxy<Block>(),
        block( _Factory().duplicate(*other.block) ),
        mutex( _Factory().query() )
        {

        }

        Natural & Natural:: plan(const Plan p) noexcept
        {
            block->to(p);
            return *this;
        }

        Natural & Natural:: plan(Random::Bits &ran) noexcept
        {
            const unsigned p = ran.in<unsigned>(Plan1,Plan8);
            return plan( Plan(p) );
        }

        Lockable & Natural:: access() const noexcept
        {
            return *mutex;
        }


        Natural & Natural:: operator=(const Natural &other)
        {
            BlockPtr temp( _Factory().duplicate( *other.block) );
            block.swp(temp);
            return *this;
        }




        Natural & Natural:: operator=(const natural_t n)  noexcept
        {


            if(block->shift>Block::MinShift)
            {
                Block *tmp = _Factory().tryQuerySmall();
                if(0!=tmp)
                {
                    BlockPtr blk(tmp);
                    blk.swp(block);
                }
            }


            block->make<NaturalPlan>().word[0] = n;
            block->sync();
            return *this;
        }

        uint64_t Natural:: lsw() const noexcept
        {
            volatile AutoLock guard(*this);
            return block->make<Plan8>().word[0];
        }




        static inline
        size_t BytesForBits(size_t nbit) noexcept
        {
            ++nbit;
            return Y_ALIGN8(nbit) >> 3;
        }

        Natural:: Natural(const Exp2_ &, const size_t nbit) :
        Castable(),
        block( _Factory().queryBytes( BytesForBits(nbit) ) ),
        mutex( _Factory().query() )
        {
            block->to(Plan1);
            block->set_(nbit);
            block->sync();
        }

        Natural & Natural:: xch(Natural &other)  noexcept
        {
            block.swp(other.block);
            return *this;
        }

    }

}


#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Natural:: Natural(const String &s) :
        Castable(),
        block( _Factory().query(NaturalShift) ),
        mutex( _Factory().query() )
        {
            block->make<NaturalPlan>().word[0] = 0;
            block->sync();

            const char *c = s.c_str();
            size_t      n = s.size(); if(n<=0) throw Specific::Exception(CallSign,"=Empty String");
            if(n>=2 && '0' == c[0] && 'x' == c[1] )
            {
                // parse hex
                n -= 2; if(n<=0) throw Specific::Exception(CallSign,"Empty HexaDecimal String");
                c += 2;
                (void) toHex(c,n);
            }
            else
            {
                // parse dec
                (void) toDec(c,n);
            }
        }

        Natural & Natural:: operator=(const String &s)
        {
            { Natural _(s); xch(_); }
            return *this;
        }



    }

}


