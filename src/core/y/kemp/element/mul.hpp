
#ifndef Y_Kemp_Element_Mul_Included
#define Y_Kemp_Element_Mul_Included 1

#include "y/kemp/element/compute.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        template <typename CORE,typename WORD>
        struct Multiplication
        {
            //__________________________________________________________________
            //
            // core algorithm for pre-allocated prod
            //__________________________________________________________________
            static inline
            size_t Run(Assembly<WORD>       &prod,
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

            //__________________________________________________________________
            //
            //
            //! New element for input metric
            //
            //__________________________________________________________________
            static inline
            Element *New(const Assembly<WORD> &l,
                                       const Assembly<WORD> &r)
            {
                return new Element( (l.positive+r.positive) * sizeof(WORD), AsCapacity);
            }

            //__________________________________________________________________
            //
            //
            //! low level compute from two assemblies
            //
            //__________________________________________________________________
            static inline
            Element *Get(const Assembly<WORD> &l,
                         const Assembly<WORD> &r)
            {
                AutoPtr<Element>  s = New(l,r);
                s->bits = Run( s->get<WORD>(), l, r);
                return s.yield();
            }

            static inline
            Element *GetEx(const Assembly<WORD> &l,
                           const Assembly<WORD> &r,
                           uint64_t             &tmx)
            {
                AutoPtr<Element>  s = New(l,r);
                Y_Kemp_TMX_Ini();
                s->bits = Run( s->get<WORD>(), l, r);
                Y_Kemp_TMX_Add();
                return s.yield();
            }

        };


    }

}

#endif

