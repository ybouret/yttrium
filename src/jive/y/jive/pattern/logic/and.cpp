#include "y/jive/pattern/logic/and.hpp"

namespace Yttrium
{
    namespace Jive
    {

        And::  And() noexcept : Compound(UUID) {}

        And::  And(const And &other) : Compound(other) {}

        And:: ~And() noexcept
        {
        }

        Pattern * And:: clone() const
        {
            return new And(*this);
        }

        void And:: query(FirstChars &fc) const
        {
            for(const Pattern *p = patterns.head; p; p=p->next)
            {
                p->query(fc);
                if(p->isRegular())
                    break;
            }
        }

        bool And:: isFragile() const noexcept
        {

            return false;
        }

        bool And:: takes(Source &source)
        {
            assert(0==size);

            for(Pattern *p=patterns.head;p;p=p->next)
            {

            }


            return false;
        }


    }

}

