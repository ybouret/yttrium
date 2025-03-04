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
                    CarryType    cr = 0;

                    //__________________________________________________________
                    //
                    // common part
                    //__________________________________________________________
                    {
                        const WORD  *r  = rhs.item;
                        for(size_t i=nr;i>0;--i)
                        {
                            cr += static_cast<CarryType>(*(l++)) - static_cast<CarryType>(*(r++));
                            if(cr<0)
                            {
                                *(s++) = static_cast<WORD>(cr+Radix);
                                cr  = -1;
                            }
                            else
                            {
                                *(s++) = static_cast<WORD>(cr);
                                cr  = 0;
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    // propagate carry on extra part
                    //__________________________________________________________
                    for(size_t i=nl-nr;i>0;--i)
                    {
                        cr += static_cast<CarryType>(*(l++));
                        if(cr<0)
                        {
                            *(s++) = static_cast<WORD>(cr+Radix);
                            cr  = -1;
                        }
                        else
                        {
                            *(s++) = static_cast<WORD>(cr);
                            cr  = 0;
                        }
                    }

                    //__________________________________________________________
                    //
                    // check last carry
                    //__________________________________________________________
                    if(cr<0) throw Libc::Exception(EDOM, "Assembly::Sub(lhs<rhs)");
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

            

        };



    }
}

#endif
