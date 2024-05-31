
#ifndef Y_Kemp_Element_Mul_Included
#define Y_Kemp_Element_Mul_Included 1

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
        // core algorithm for pre-allocated prod
        //______________________________________________________________________
        template <typename CORE,typename WORD>
        static inline
        size_t AssemblyMulStd(Assembly<WORD>       &prod,
                              const Assembly<WORD> &lhs,
                              const Assembly<WORD> &rhs) noexcept
        {
            Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
            assert(prod.capacity>=lhs.positive+rhs.positive);
            assert(0==prod.positive);
            const size_t p=lhs.positive;
            const size_t q=rhs.positive;
            if(p<=0||q<=0) return 0;

            const WORD * const a = lhs.item;
            const WORD *       b = rhs.item;
            WORD       *       product = prod.item;
            for(size_t j=q;j>0;--j,++product)
            {
                const CORE B     = static_cast<CORE>(*(b++));
                CORE       carry = 0;
                for(size_t i=0;i<p;++i)
                {
                    carry += static_cast<CORE>(product[i]) + static_cast<CORE>(a[i]) * B;
                    product[i] = static_cast<WORD>(carry);
                    carry >>= Assembly<WORD>::WordBits;
                }
                product[p] =  static_cast<WORD>(carry);
            }

            prod.positive = p+q;
            return prod.updateBits();
        }


        template <typename CORE, typename WORD>
        inline Element * ElementMulStd(Element &lhs,
                                       Element &rhs)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            AutoPtr<Element>  s = new Element( (l.positive+r.positive) * sizeof(WORD), AsCapacity);
            s->bits = AssemblyMulStd<CORE>( s->get<WORD>(), l, r);
            return s.yield();
        }

        template <typename CORE, typename WORD>
        inline Element * ElementMulStdEx(Element &lhs,
                                         Element &rhs,
                                         uint64_t &tmx)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            AutoPtr<Element>  s = new Element( (l.positive+r.positive) * sizeof(WORD), AsCapacity);
            const uint64_t    t = WallTime::Ticks();
            s->bits = AssemblyMulStd<CORE>( s->get<WORD>(), l, r);
            tmx += (WallTime::Ticks() - t);
            return s.yield();
        }



    }

}

#endif

