#ifndef Y_Kemp_Element_Sub_Included
#define Y_Kemp_Element_Sub_Included 1

#include "y/kemp/element/compute-binary.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //! Long Subtraction
        //
        //______________________________________________________________________
        template <typename CORE, typename WORD>
        struct Subtraction
        {
            //__________________________________________________________________
            //
            //! core algorithm for pre-allocated sub
            //__________________________________________________________________
            static inline
            size_t Run(Assembly<WORD>       &sub,
                       const Assembly<WORD> &lhs,
                       const Assembly<WORD> &rhs)
            {
                Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
                typedef typename SignedInt<sizeof(CORE)>::Type CarryType;
                static  const CarryType Radix =  CarryType(1) << (sizeof(WORD) << 3);

                assert(sub.capacity>=lhs.positive);
                assert(lhs.positive>=rhs.positive);

                const size_t nl = lhs.positive;
                {
                    const WORD  *l  = lhs.item;
                    const size_t nr = rhs.positive;
                    WORD  *      s  = sub.item;
                    CarryType carry = 0;

                    //__________________________________________________________
                    //
                    // common part
                    //__________________________________________________________
                    {
                        const WORD  *r  = rhs.item;
                        for(size_t i=nr;i>0;--i)
                        {
                            carry += static_cast<CarryType>(*(l++)) - static_cast<CarryType>(*(r++));
                            if(carry<0)
                            {
                                *(s++) = static_cast<WORD>(carry+Radix);
                                carry  = -1;
                            }
                            else
                            {
                                *(s++) = static_cast<WORD>(carry);
                                carry  = 0;
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    // propagate carry
                    //__________________________________________________________
                    for(size_t i=nl-nr;i>0;--i)
                    {
                        carry += static_cast<CarryType>(*(l++));
                        if(carry<0)
                        {
                            *(s++) = static_cast<WORD>(carry+Radix);
                            carry  = -1;
                        }
                        else
                        {
                            *(s++) = static_cast<WORD>(carry);
                            carry  = 0;
                        }
                    }
                    if(carry<0) throw Libc::Exception(EDOM, "Assembly::Sub(lhs<rhs)");
                }


                sub.positive = nl;
                return sub.updateBits();
            }

            //__________________________________________________________________
            //
            //! new empty element matching the lhs positive
            //__________________________________________________________________
            static inline Element *New(const Assembly<WORD> &l,
                                       const Assembly<WORD> &r)
            {
                if(l.positive<r.positive) throw Libc::Exception(EDOM, "%s::Sub(lhs<rhs)", Element::CallSign);
                return new Element( l.positive * sizeof(WORD), AsCapacity);
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
                return Element::Shrink(s.yield()->revise());
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
                return Element::Shrink(s.yield()->revise());
            }

        };



    }
}

#endif
