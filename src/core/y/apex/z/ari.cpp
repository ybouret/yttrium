#include "y/apex/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer Integer:: Sqrt(const Integer &z)
        {
            switch(z.s)
            {
                case Negative: throw Specific::Exception(CallSign,"negative square root");
                case __Zero__: return Integer();
                case Positive: break;
            }
            const  Natural sq = z.n.sqrt();
            return Integer(Positive,sq);
        }

        Integer Integer:: sqrt() const
        {
            return Sqrt(*this);
        }

        Integer Integer:: Abs(const Integer &z)
        {
            switch(z.s)
            {
                case __Zero__:
                case Positive:
                    return z;
                case Negative:
                    break;
            }
            return Integer(Positive,z.n);
        }

        Integer Integer:: abs() const
        {
            return Abs(*this);
        }

    }

}

