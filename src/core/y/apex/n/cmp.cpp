#include "y/apex/natural.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        bool operator==(const Natural &lhs, const Natural &rhs) noexcept
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(rhs);
            const Jig1 &l = lhs.block->make<Plan1>(); const size_t n = l.words;
            const Jig1 &r = rhs.block->make<Plan1>(); if(n!=r.words) return false;
            return 0 == memcmp(l.word,r.word,n);
        }

        bool operator==(const Natural & lhs, const natural_t rhs) noexcept
        {
            volatile Natural::AutoLock L(lhs);
            const Jig8 &l = lhs.block->make<Plan8>();
            return (l.words <= 1) && (rhs == l.word[0]);
        }

        bool operator==(const natural_t lhs, const Natural & rhs) noexcept
        {
            volatile Natural::AutoLock R(rhs);
            const Jig8 &r  = rhs.block->make<Plan8>();
            return (r.words <= 1) && (lhs == r.word[0]);
        }



        bool operator!=(const Natural &lhs, const Natural &rhs) noexcept
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(rhs);
            const Jig1 &l = lhs.block->make<Plan1>(); const size_t n = l.words;
            const Jig1 &r = rhs.block->make<Plan1>(); if(n!=r.words) return false;
            return 0 != memcmp(l.word,r.word,n);
        }


        bool operator!=(const Natural & lhs, const natural_t rhs) noexcept
        {
            volatile Natural::AutoLock L(lhs);
            const Jig8 &l = lhs.block->make<Plan8>();
            return (l.words > 1) || (rhs != l.word[0]);
        }


        bool operator!=(const natural_t lhs, const Natural & rhs) noexcept
        {
            volatile Natural::AutoLock R(rhs);
            const Jig8 &r  = rhs.block->make<Plan8>();
            return (r.words > 1) && (lhs != r.word[0]);
        }


        SignType Natural:: Compare(const Natural & lhs, const Natural &rhs) noexcept
        {
            volatile Natural::AutoLock L(lhs);
            volatile Natural::AutoLock R(rhs);
            const Jig8 &l = lhs.block->make<Plan8>();
            const Jig8 &r = rhs.block->make<Plan8>();

            switch( Sign::Of(l.words,r.words) )
            {
                case Negative: return Negative; // l.words < r.words
                case Positive: return Positive; // l.words > r.words
                case __Zero__:
                    break;
            }

            size_t n = l.words;
            while(n-- > 0)
            {
                switch( Sign::Of(l.word[n],r.word[n]) )
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__: continue;
                }
            }

            return __Zero__;
        }

    }

}
