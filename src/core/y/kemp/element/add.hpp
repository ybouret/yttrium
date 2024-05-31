#ifndef Y_Kemp_Element_Add_Included
#define Y_Kemp_Element_Add_Included 1

#include "y/kemp/element.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //______________________________________________________________________
        //
        // core algorithm for pre-allocated sum
        //______________________________________________________________________
        template <typename CORE,typename WORD>
        static inline
        size_t AssemblyAdd(Assembly<WORD>       &sum,
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

        template <typename CORE, typename WORD>
        inline Element * ElementAdd(Element &lhs,
                                    Element &rhs)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            AutoPtr<Element>  s = new Element( (Max(l.positive,r.positive)+1) * sizeof(WORD), AsCapacity);
            s->bits = AssemblyAdd<CORE>( s->get<WORD>(), l, r);
            return s.yield();
        }

        template <typename CORE, typename WORD>
        inline Element * ElementAddEx(Element  &lhs,
                                      Element  &rhs,
                                      uint64_t &tmx)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            AutoPtr<Element>  s = new Element( (Max(l.positive,r.positive)+1) * sizeof(WORD), AsCapacity);
            const uint64_t    t = WallTime::Ticks();
            s->bits = AssemblyAdd<CORE>( s->get<WORD>(), l, r);
            tmx += (WallTime::Ticks() - t);
            return s.yield();
        }



        
    }
}

#endif

