
#include "y/kemp/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Integer Integer:: Square(const Integer &z)
        {
            const Natural z2 = z.n.square();
            return Integer(Positive,z2);
        }

        Integer Integer:: square() const
        {
            return Square(*this);
        }

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
            Integer a = z;
            Sign::ReplaceByAbs(Coerce(a.s));
            return a;
        }

        Integer Integer:: abs() const
        {
            return Abs(*this);
        }
    }
    
}
