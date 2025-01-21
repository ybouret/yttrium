
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Natural:: AutoLock:: AutoLock(const Natural &n) noexcept :
        host(n)
        {
            host->lock();
        }

        Natural:: AutoLock:: ~AutoLock() noexcept {
            host->unlock();
        }


        const char * const Natural:: CallSign = "Apex::Natural";

        const AddOps Natural:: AddOpsTable[NumAddOps] =
        {
            Y_Apex_AddOps_List
        };

        const char * const Natural:: AddOpsLabel[NumAddOps] =
        {
            "Add8_1",
            "Add8_2",
            "Add8_4",
            "Add4_1",
            "Add4_2",
            "Add2_1"
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
            const Block::Momentary change(Plan1,*n.block);
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


    }

}


