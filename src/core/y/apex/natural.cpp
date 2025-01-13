
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
        typedef Jig<NaturalPlan> NaturalJig;

        Natural:: Natural(const natural_t value) :
        block( _Factory().acquire(NaturalShift) )
        {
            block->make<NaturalPlan>().word[0] = value;
            block->sync();
        }

        Natural:: ~Natural() noexcept
        {
            assert(0!=block);
            static Factory &factory = Factory::Location();
            factory.release(block);
            block = 0;
        }

        Natural:: Natural(const Natural &_) :
        block( _Factory().duplicate(_.block) )
        {
            
        }


        std::ostream & operator<<(std::ostream &os, const Natural &n) {
            const Block::Briefly change(Plan1,*n.block);
            os << *n.block;
            return os;
        }

        Natural & Natural:: operator=(const Natural &other)
        {
            Natural _(other);
            Swap(block,_.block);
            return *this;
        }



    }

}


