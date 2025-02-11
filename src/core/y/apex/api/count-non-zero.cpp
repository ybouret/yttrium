#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Apex
    {

        size_t CountNonZero_<Natural>:: In(const Readable<Natural> &arr) noexcept
        {
            size_t res = 0;
            for(size_t i=arr.size();i>0;--i)
                if(arr[i]->bits>0) ++res;
            return res;
        }

        size_t CountNonZero_<Integer>:: In(const Readable<Integer> &arr) noexcept
        {
            size_t res = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                switch(arr[i].s)
                {
                    case __Zero__: continue;
                    case Positive:
                    case Negative: ++res;
                        break;
                }
            }
            return res;
        }

        size_t CountNonZero_<Rational>:: In(const Readable<Rational> &arr) noexcept
        {
            size_t res = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                switch(arr[i].numer.s)
                {
                    case __Zero__: continue;
                    case Positive:
                    case Negative: ++res;
                        break;
                }
            }
            return res;
        }

    }

}

