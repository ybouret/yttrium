

#ifndef Y_Kemp_Element_Mul_Included
#define Y_Kemp_Element_Mul_Included 1

#include "y/kemp/element/compute-unary.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //! Long Mutliplication
        //
        //______________________________________________________________________
        template <typename CORE,typename WORD>
        struct ComputeSquare
        {
            
            //__________________________________________________________________
            //
            //
            //! core algorithm for pre-allocated prod
            //
            //__________________________________________________________________
            static inline
            size_t Run(Assembly<WORD>       &prod,
                       const Assembly<WORD> &args) noexcept
            {
                Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
                assert(prod.capacity>=args.positive*2);
                assert(0==prod.positive);
                const size_t n=args.positive;
                if(n<=0) return 0;

                const WORD * const a = args.item;
                const WORD *       b = args.item;
                WORD       *       product = prod.item;
                for(size_t j=n;j>0;--j,++product)
                {
                    const CORE B     = static_cast<CORE>(*(b++));
                    CORE       carry = 0;
                    for(size_t i=0;i<n;++i)
                    {
                        carry += static_cast<CORE>(product[i]) + static_cast<CORE>(a[i]) * B;
                        product[i] = static_cast<WORD>(carry);
                        carry >>= Assembly<WORD>::WordBits;
                    }
                    product[n] =  static_cast<WORD>(carry);
                }

                prod.positive = n<<1;
                return prod.updateBits();
            }

            //__________________________________________________________________
            //
            //
            //! New element for input metrics
            //
            //__________________________________________________________________
            static inline
            Element *New(const Assembly<WORD> &args)
            {
                return new Element( (args.positive<<1) * sizeof(WORD), AsCapacity);
            }

            //__________________________________________________________________
            //
            //
            //! low level compute from two assemblies
            //
            //__________________________________________________________________
            static inline
            Element *Get(const Assembly<WORD> &args)
            {
                AutoPtr<Element>  s = New(args);
                s->bits = Run( s->get<WORD>(), args);
                return s.yield()->revise();
            }
            

        };

    }

}

#endif

