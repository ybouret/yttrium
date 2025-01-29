
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        Integer Integer:: operator-() const
        {
            Integer _(*this);
            Sign::ReplaceByOpposite( Coerce(_.s) );
            return _;
        }

        Integer Integer:: Sub(const Integer &lhs, const Integer &rhs)
        {
            
        }


        void Integer:: decr()
        {
            switch(s)
            {
                case Negative: Coerce(n).incr(); break;

                case __Zero__:
                    Coerce(s) = Negative;
                    Coerce(n) = 1;
                    break;

                case Positive:
                    if(1==n->bits)
                    {
                        Coerce(s) = __Zero__;
                        Coerce(n) = 0;
                    }
                    else
                    {
                        Coerce(n).decr();
                    }
            }
        }

        Integer & Integer:: operator--()
        {
            decr();
            return *this;
        }

        Integer Integer:: operator--(int)
        {
            const Integer old(*this);
            decr();
            return old;
        }

    }

}
