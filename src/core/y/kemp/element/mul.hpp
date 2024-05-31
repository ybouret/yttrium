
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
        size_t AssemblyMulSTD(Assembly<WORD>       &prod,
                              const Assembly<WORD> &lhs,
                              const Assembly<WORD> &rhs) noexcept
        {
            Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
            assert(prod.capacity>=lhs.positive+rhs.positive);
            assert(0==prod.positive);
            const size_t p=lhs.positive;
            const size_t q=rhs.positive;
            if(p<=0||q<=0) return 0;

            const WORD * const a = lhs.item-1;
            const WORD * const b = rhs.item-1;
            WORD       * const product = prod.item-1;

            for(size_t j=1;j<=q;++j)
            {
                CORE carry = 0;
                for(size_t i=1;i<=p;++i)
                {
                    carry += static_cast<CORE>(product[i+j-1]) + static_cast<CORE>(a[i]) * static_cast<CORE>(b[j]);
                    product[i+j-1] = static_cast<WORD>(carry);
                    carry >>= Assembly<WORD>::WordBits;
                }
                product[j + p] =  static_cast<WORD>(carry);
            }

            prod.positive = p+q;
            return prod.updateBits();
        }


        template <typename CORE, typename WORD>
        inline Element * ElementMulSTD(Element &lhs,
                                       Element &rhs)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            AutoPtr<Element>  s = new Element( (l.positive+r.positive) * sizeof(WORD), AsCapacity);
            s->bits = AssemblyMulSTD<CORE>( s->get<WORD>(), l, r);
            return s.yield();
        }

    }

}

#endif

