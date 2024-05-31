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
            WORD *   s     = sum.item;
            CORE     carry = 0;

            //__________________________________________________________________
            //
            // select big/little
            //__________________________________________________________________
            const Assembly<WORD> *big = &lhs;
            const Assembly<WORD> *lit = &rhs;
            if(big->positive<lit->positive) Swap(lit,big);
            assert(lit->positive<=big->positive);
            size_t       q = big->positive;
            assert(sum.capacity>q);
            {
                const WORD * b = big->item;
                const size_t p = lit->positive; assert(p<=q);
                //______________________________________________________________
                //
                // sum over shared size
                //______________________________________________________________
                {
                    const WORD * a = lit->item;
                    for(size_t i=p;i>0;--i)
                    {
                        carry  += static_cast<CORE>(*(a++)) + static_cast<CORE>(*(b++));
                        *(s++)  = static_cast<WORD>(carry);
                        carry >>= Assembly<WORD>::WordBits;
                    }
                }

                //______________________________________________________________
                //
                // sum over remaining size
                //______________________________________________________________
                for(size_t i=q++ - p;i>0;--i)
                {
                    carry += static_cast<CORE>(*(b++));
                    *(s++) = static_cast<WORD>(carry);
                    carry >>= Assembly<WORD>::WordBits;
                }
            }

            //__________________________________________________________________
            //
            // finalize
            //__________________________________________________________________
            *s = static_cast<WORD>(carry);
            sum.positive = q;

            return sum.updateBits();
        }

        template <typename CORE,typename WORD>
        inline Element * ElementAdd(const Assembly<WORD> &l,
                                    const Assembly<WORD> &r)
        {
            AutoPtr<Element>  s = new Element( (Max(l.positive,r.positive)+1) * sizeof(WORD), AsCapacity);
            s->bits = AssemblyAdd<CORE>( s->get<WORD>(), l, r);
            return s.yield();
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

