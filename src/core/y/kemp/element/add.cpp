
#include "y/kemp/element.hpp"
#include "y/type/utils.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        template <typename CORE,typename WORD>
        static inline
        size_t AddAssembly(Assembly<WORD>       &sum,
                           const Assembly<WORD> &lhs,
                           const Assembly<WORD> &rhs) noexcept
        {
            Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
            assert(0==sum.positive);
            WORD * const s     = sum.item;
            CORE         carry = 0;

            // select big/little
            const Assembly<WORD> *big = &lhs;
            const Assembly<WORD> *lit = &rhs;
            if(big->positive<lit->positive) Swap(lit,big);
            assert(lit->positive<=big->positive);

            const WORD * const a = lit->item;
            const size_t       p = lit->positive;

            const WORD * const b = big->item;
            const size_t       q = big->positive;

            assert(p<=q);
            assert(sum.capacity>q);

            // sum over shared size
            for(size_t i=0;i<p;++i)
            {
                carry  += static_cast<CORE>(a[i]) + static_cast<CORE>(b[i]);
                s[i]    = static_cast<WORD>(carry);
                carry >>= Assembly<WORD>::WordBits;
            }

            // sum over remaining size
            for(size_t i=p;i<q;++i)
            {
                carry += static_cast<CORE>(b[i]);
                s[i]   = static_cast<WORD>(carry);
                carry >>= Assembly<WORD>::WordBits;
            }

            // finalize
            s[q] = static_cast<WORD>(carry);
            sum.positive = q+1;

            
            return sum.updateBits();
        }


        template <>
        Element * Element:: Add<uint64_t,uint32_t>(Element &lhs,
                                                   Element &rhs)
        {
            const Num32     &l = lhs.set(AsNum32).num32;
            const Num32     &r = rhs.set(AsNum32).num32;
            const size_t     n = Max(l.positive,r.positive)+1;
            AutoPtr<Element> s = new Element(n*sizeof(uint32_t),AsCapacity);
            s->bits = AddAssembly<uint64_t,uint32_t>(s->set(AsNum32).num32,l,r);
            return s.yield();
        }

    }

}
