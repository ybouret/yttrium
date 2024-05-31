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
        struct SubErr
        {
            static const char Message[];
            static const char Level1[];
            static const char Level2[];
        };
        //______________________________________________________________________
        //
        // core algorithm for pre-allocated sum
        //______________________________________________________________________
        template <typename CORE,typename WORD>
        static inline
        size_t AssemblySub(Assembly<WORD>       &sub,
                           const Assembly<WORD> &lhs,
                           const Assembly<WORD> &rhs) noexcept
        {
            Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
            typedef SignedInt<sizeof(CORE)> CarryType;

            assert(sub.capacity>=lhs.positive);
            assert(lhs.positive>=rhs.positive);

            const WORD  *l  = lhs.item;
            const size_t nl = lhs.positive;
            const WORD  *r  = rhs.item;
            const size_t nr = rhs.positive;
            

            return 0;
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
            return s.yield();
        }

    }
}

#endif
