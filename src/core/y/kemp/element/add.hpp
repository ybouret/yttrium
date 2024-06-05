#ifndef Y_Kemp_Element_Add_Included
#define Y_Kemp_Element_Add_Included 1

#include "y/kemp/element/compute-binary.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //
        //! Addition
        //
        //
        //______________________________________________________________________
        template <typename CORE, typename WORD>
        struct Addition
        {
            //__________________________________________________________________
            //
            //
            //! core algorithm for pre-allocated sum
            //
            //__________________________________________________________________
            static inline
            size_t Run(Assembly<WORD>       &sum,
                       const Assembly<WORD> &lhs,
                       const Assembly<WORD> &rhs) noexcept
            {
                Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
                assert(0==sum.positive);
                WORD *   s     = sum.item;
                CORE     carry = 0;

                //______________________________________________________________
                //
                // select big/little
                //______________________________________________________________
                const Assembly<WORD> *big = &lhs;
                const Assembly<WORD> *lit = &rhs;
                if(big->positive<lit->positive) Swap(lit,big);
                assert(lit->positive<=big->positive);
                size_t       q = big->positive;
                assert(sum.capacity>q);
                {
                    const WORD * b = big->item;
                    const size_t p = lit->positive; assert(p<=q);
                    //__________________________________________________________
                    //
                    // sum over shared size
                    //__________________________________________________________
                    {
                        const WORD * a = lit->item;
                        for(size_t i=p;i>0;--i)
                        {
                            carry  += static_cast<CORE>(*(a++)) + static_cast<CORE>(*(b++));
                            *(s++)  = static_cast<WORD>(carry);
                            carry >>= Assembly<WORD>::WordBits;
                        }
                    }

                    //__________________________________________________________
                    //
                    // sum over remaining size
                    //__________________________________________________________
                    for(size_t i=q++ - p;i>0;--i)
                    {
                        carry += static_cast<CORE>(*(b++));
                        *(s++) = static_cast<WORD>(carry);
                        carry >>= Assembly<WORD>::WordBits;
                    }
                }

                //______________________________________________________________
                //
                // finalize
                //______________________________________________________________
                *s = static_cast<WORD>(carry);
                sum.positive = q;

                return sum.updateBits();
            }

            //__________________________________________________________________
            //
            //
            //! New element for input metric
            //
            //__________________________________________________________________
            static inline Element *New(const Assembly<WORD> &l,
                                       const Assembly<WORD> &r)
            {
                return new Element( (Max(l.positive,r.positive)+1) * sizeof(WORD), AsCapacity);
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
                return s.yield()->revise();
            }

            //__________________________________________________________________
            //
            //
            //! low level compute from two assemblies with timing
            //
            //__________________________________________________________________
            static inline
            Element *GetEx(const Assembly<WORD> &l,
                           const Assembly<WORD> &r,
                           uint64_t             &tmx)
            {
                AutoPtr<Element>  s = New(l,r);
                Y_Kemp_TMX_Ini();
                s->bits = Run( s->get<WORD>(), l, r);
                Y_Kemp_TMX_Add();
                return s.yield()->revise();
            }
            
        };

     




    }
}

#endif

