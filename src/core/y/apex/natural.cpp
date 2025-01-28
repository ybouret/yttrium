
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
        const char * Natural:: callSign() const noexcept { return CallSign; }



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

        Natural:: Natural(const natural_t n) :
        Number(),
        block( _Factory().query(NaturalShift) ),
        mutex( _Factory().query() )
        {
            block->make<NaturalPlan>().word[0] = n;
            block->sync();
        }

        Natural:: Natural(Random::Bits &ran, const size_t nbit):
        Number(),
        block( _Factory().query(ran,nbit) ),
        mutex( _Factory().query() )
        {
            assert(nbit==block->bits);
        }

        Natural:: Natural(Block * const userBlock, const AsBlock_ &) :
        block( userBlock ),
        mutex( _Factory().query() )
        {

        }



        Natural:: ~Natural() noexcept {

        }

        Natural:: Natural(const Natural &other) :
        Number(),
        Proxy<Block>(),
        block( _Factory().duplicate(*other.block) ),
        mutex( _Factory().query() )
        {
            
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
        Number(),
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

#if 0
        size_t Natural:: bits() const noexcept
        {
            return block->bits;
        }
#endif
        
    }

}


