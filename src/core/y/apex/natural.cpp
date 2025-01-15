
#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static inline Factory & _Factory()
        {
            static Factory & _ = Factory::Instance();
            return _;
        }

        static const unsigned    NaturalShift = iLog2Of<natural_t>::Value;
        static const Plan        NaturalPlan  = Plan(NaturalShift);

        Natural:: Natural(const natural_t value) :
        Number(),
        block( _Factory().query(NaturalShift) ),
        mutex( _Factory().query() )
        {
            block->make<NaturalPlan>().word[0] = value;
            block->sync();
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


        std::ostream & operator<<(std::ostream &os, const Natural &n) {
            Y_LOCK(*n);
            const Block::Briefly change(Plan1,*n.block);
            os << *n.block;
            return os;
        }

        Natural & Natural:: operator=(const Natural &other)
        {
            BlockPtr temp( _Factory().duplicate( *other.block) );
            block.swp(temp);
            return *this;
        }



    }

}


