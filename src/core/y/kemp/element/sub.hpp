#ifndef Y_Kemp_Element_Sub_Included
#define Y_Kemp_Element_Sub_Included 1

#include "y/kemp/element.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

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
        size_t AssemblySub(Assembly<WORD>       &sub,
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

                //______________________________________________________________
                //
                // common part
                //______________________________________________________________
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

                //______________________________________________________________
                //
                // propagate carry
                //______________________________________________________________
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

        template <typename CORE, typename WORD>
        inline Element * ElementSub(Element &lhs,
                                    Element &rhs)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            if(l.positive<r.positive) throw Libc::Exception(EDOM, "%s::Sub(lhs<rhs)", Element::CallSign);
            AutoPtr<Element>  s = new Element(l.positive*sizeof(WORD), AsCapacity);
            s->bits = AssemblySub<CORE>( s->get<WORD>(), l, r);
            return Element::Shrink(s.yield());
        }

        template <typename CORE, typename WORD>
        inline Element * ElementSubEx(Element  &lhs,
                                      Element  &rhs,
                                      uint64_t &tmx)
        {
            typedef Assembly<WORD> AssemblyType;
            AssemblyType     &l = lhs.get<WORD>();
            AssemblyType     &r = rhs.get<WORD>();
            if(l.positive<r.positive) throw Libc::Exception(EDOM, "%sEx::Sub(lhs<rhs)", Element::CallSign);
            AutoPtr<Element>  s = new Element(l.positive*sizeof(WORD), AsCapacity);
            const uint64_t    t = WallTime::Ticks();
            s->bits = AssemblySub<CORE>(s->get<WORD>(), l, r);
            tmx += (WallTime::Ticks()-t);
            return Element::Shrink(s.yield());
        }

    }
}

#endif
