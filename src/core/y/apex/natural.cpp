
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Y_SHALLOW_IMPL(AsBlock);
        Y_SHALLOW_IMPL(Exp2);


        Natural:: AutoLock:: AutoLock(const Natural &n) noexcept :
        host(n)
        {
            host->lock();
        }

        Natural:: AutoLock:: ~AutoLock() noexcept {
            host->unlock();
        }


        const char * const Natural:: CallSign = "Apex::Natural";


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
        block( _Factory().duplicate(*other.block) ),
        mutex( _Factory().query() )
        {
            
        }




        Lockable & Natural:: operator*() const noexcept
        {
            return *mutex;
        }

        
        Lockable * Natural::operator->() const noexcept
        {
            return & *mutex;
        }


        Block &       Natural:: _block() noexcept
        {
            return *block;
        }

        const Block & Natural:: _block() const noexcept
        {
            return *block;
        }


        std::ostream & operator<<(std::ostream &os, const Natural &n) {
            Y_LOCK(*n);
            n.block->to(Plan1);
            os << *n.block;
            return os;
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
            static const uint8_t _bit[8] = { 1,2,4,8,16,32,64,128 };
            block->make<Plan1>().word[nbit >> 3] = _bit[nbit &  7];
            block->sync();
        }


    }

}


